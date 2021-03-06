/* 
 * Copyright (C) 2005 MaNGOS <http://www.magosproject.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "UpdateMask.h"
#include "Unit.h"

void WorldSession::HandleSendMail(WorldPacket & recv_data )
{
    time_t base = time(NULL);
    time_t etime = base + (30 * 3600);
    WorldPacket data;
    uint64 sender,item;
    std::string reciever,subject,body;
    uint32 unk1,unk2,money,COD,mID;
    recv_data >> sender;
    recv_data >> reciever >> subject >> body;
    recv_data >> unk1 >> unk2;
    recv_data >> item;
    recv_data >> money >> COD;

    Log::getSingleton().outString("Player %u is sending mail to %s with subject %s and body %s includes item %u and %u copper and %u COD copper",GUID_LOPART(sender),reciever.c_str(),subject.c_str(),body.c_str(),GUID_LOPART(item),money,COD);
    mID = objmgr.GenerateMailID();

    Player* pl = GetPlayer();

    WorldPacket tmpData;                    
    uint32 tmpMoney = pl->GetUInt32Value(PLAYER_FIELD_COINAGE); 

    if (tmpMoney - money < 30)    
    {             
        tmpData.Initialize(SMSG_SEND_MAIL_RESULT);
        tmpData << uint32(0);
        tmpData << uint32(0);
        tmpData << uint32(3);
        SendPacket(&tmpData);    
    }
    else
    {
        std::stringstream ss;
        ss << "SELECT guid FROM characters WHERE name = '" << reciever << "'";
        QueryResult *result = sDatabase.Query( ss.str( ).c_str( ) );

		Player *recieve = objmgr.GetPlayer(reciever.c_str());
        uint64 rc = objmgr.GetPlayerGUIDByName(reciever.c_str());
        if (result)
        {
			delete result;

			data.Initialize(SMSG_SEND_MAIL_RESULT);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            SendPacket(&data);

			if (item != 0)
            {
                uint32 slot = pl->GetSlotByItemGUID(item);
                Item *it = pl->GetItemBySlot((uint8)slot);
                objmgr.AddMItem(it);

                std::stringstream ss;
                ss << "INSERT INTO mailed_items (guid, data) VALUES ("
                   << it->GetGUIDLow() << ", '";         
                for(uint16 i = 0; i < it->GetValuesCount(); i++ )
                {
                   ss << it->GetUInt32Value(i) << " ";
                }
                ss << "' )";
                sDatabase.Execute( ss.str().c_str() );

                pl->RemoveItemFromSlot((uint8)slot);
            }
            uint32 playerGold = pl->GetUInt32Value(PLAYER_FIELD_COINAGE);
            pl->SetUInt32Value( PLAYER_FIELD_COINAGE, playerGold - 30 - money );
        
            if (recieve)
            {
                Mail* m = new Mail;
                m->messageID = mID;
                m->sender =   pl->GetGUIDLow();  
                m->reciever = GUID_LOPART(rc);
                m->subject = subject;
                m->body = body;
                m->item = GUID_LOPART(item);
                m->money = money;
                m->time = etime;
                m->COD = 0;
                m->checked = 0;
                recieve->AddMail(m);

		        
		        data.Initialize(SMSG_RECEIVED_MAIL);
                data << uint32(0);
                SendPacket(&data);
		        recieve->GetSession()->SendPacket(&data);
            }

            std::stringstream delinvq;
            
            delinvq << "DELETE FROM mail WHERE mailID = " << mID;
            sDatabase.Execute( delinvq.str().c_str( ) );
            std::stringstream ss;
            ss << "INSERT INTO mail (mailId,sender,reciever,subject,body,item,time,money,COD,checked) VALUES ( " <<
                mID << ", " << pl->GetGUIDLow() << ", " << GUID_LOPART(rc) << ",' " << subject.c_str() << "' ,' " <<
            body.c_str() << "', " << GUID_LOPART(item) << ", " << (long)etime << ", " << money << ", " << 0 << ", " << 0 << " )";
            sDatabase.Execute( ss.str().c_str( ) );

        }     
		else
		{
            data.Initialize(SMSG_SEND_MAIL_RESULT);
            data << uint32(0);
            data << uint32(0);
            data << uint32(4);
            SendPacket(&data);
		}
    }
}


void WorldSession::HandleMarkAsRead(WorldPacket & recv_data )
{
    uint64 mailbox;
    uint32 message;
    recv_data >> mailbox;
    recv_data >> message;
    Player *pl = GetPlayer();
    Mail *m = pl->GetMail(message);
    m->checked = 1;
    m->time = time(NULL) + (3 * 3600);
    pl->AddMail(m);
}


void WorldSession::HandleMailDelete(WorldPacket & recv_data )
{
    uint64 mailbox;
    uint32 message;
    WorldPacket data;
    recv_data >> mailbox;
    recv_data >> message;
    Player *pl = GetPlayer();
    Mail *m = pl->GetMail(message);
    if (m->item != 0)
    {
        objmgr.RemoveMItem(m->item);
    }
    pl->RemoveMail(message);

    data.Initialize(SMSG_SEND_MAIL_RESULT);
    data << uint32(message);
    data << uint32(4);
    data << uint32(0);
    SendPacket(&data);

}


void WorldSession::HandleReturnToSender(WorldPacket & recv_data )
{
    uint64 mailbox;
    uint32 message;
    WorldPacket data;
    recv_data >> mailbox;
    recv_data >> message;
    Player *pl = GetPlayer();
    Mail *m = pl->GetMail(message);
    m->reciever = m->sender;
    m->sender = pl->GetGUIDLow();
    m->time = sWorld.GetGameTime() + (30 * 3600);
    m->checked = 0;
    pl->RemoveMail(message);

    data.Initialize(SMSG_SEND_MAIL_RESULT);
    data << uint32(message);
    data << uint32(3);
    data << uint32(0);
    SendPacket(&data);

    uint64 rc;
    GUID_LOPART(rc) = m->reciever;
    GUID_HIPART(rc) = 0;
    std::string name;
    objmgr.GetPlayerNameByGUID(rc,name);
    Player *recieve = objmgr.GetPlayer(name.c_str());
    if (recieve)
    {
        recieve->AddMail(m);
    }

    std::stringstream delinvq;
    
    delinvq << "DELETE FROM mail WHERE mailID = " << m->messageID;
    sDatabase.Execute( delinvq.str().c_str( ) );
    std::stringstream ss;
    ss << "INSERT INTO mail (mailId,sender,reciever,subject,body,item,time,money,COD,checked) VALUES ( " <<
        m->messageID << ", " << pl->GetGUIDLow() << ", " << m->reciever << ",' " << m->subject.c_str() << "' ,' " <<
        m->body.c_str() << "', " << m->item << ", " << (long)m->time << ", " << m->money << ", " << 0 << ", " << m->checked << " )";
    sDatabase.Execute( ss.str().c_str( ) );

}


void WorldSession::HandleTakeItem(WorldPacket & recv_data )
{
    uint8 i,slot;
    uint64 mailbox;
    uint32 message;
    WorldPacket data;
    recv_data >> mailbox;
    recv_data >> message;
    Player* pl = GetPlayer();
    Mail* m = pl->GetMail(message);
    Item *it = objmgr.GetMItem(m->item);
    m->item = 0;
    pl->AddMail(m);
    for(i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
    {
        if (GetPlayer()->GetItemBySlot(i) == NULL)
        {
            slot = i;
            break;
        }
    }
    it->SetUInt64Value(ITEM_FIELD_CONTAINED,pl->GetGUID());
    it->SetUInt64Value(ITEM_FIELD_OWNER,pl->GetGUID());
    GetPlayer()->AddItemToSlot(slot, it);
    objmgr.RemoveMItem(it->GetGUIDLow());
    data.Initialize(SMSG_SEND_MAIL_RESULT);
    data << uint32(message);
    data << uint32(2);
    data << uint32(0);
    SendPacket(&data);
}


void WorldSession::HandleTakeMoney(WorldPacket & recv_data )
{
    WorldPacket data;
    uint64 mailbox;
    uint32 id;
    recv_data >> mailbox;
    recv_data >> id;
    Player *pl = GetPlayer();
    Mail* m = pl->GetMail(id);
    uint32 money = pl->GetUInt32Value(PLAYER_FIELD_COINAGE);

    data.Initialize(SMSG_SEND_MAIL_RESULT);
    data << uint32(id);
    data << uint32(1);
    data << uint32(0);
    SendPacket(&data);

    pl->SetUInt32Value(PLAYER_FIELD_COINAGE,money + m->money);
    m->money = 0;
    pl->AddMail(m);

}


void WorldSession::HandleGetMail(WorldPacket & recv_data )
{
    uint32 info;
    recv_data >> info;
    WorldPacket data;
    Player* pl = GetPlayer();
    data.Initialize(SMSG_MAIL_LIST_RESULT);
    data << uint8(pl->GetMailSize());
    std::list<Mail*>::iterator itr;
    for (itr = pl->GetmailBegin(); itr != pl->GetmailEnd();itr++)
    {
        data << uint32((*itr)->messageID);
        data << uint8(0);
        data << uint32((*itr)->sender);
        data << uint32(0);                        
      
        data << (*itr)->subject.c_str();
        if((*itr)->body.c_str()!=NULL)            
            data << uint32((*itr)->messageID);
        else
            data << uint32(0);                    
        data << uint32(0);                        
        data << uint32(41);                       
        if ((*itr)->item != 0)
        {
            Item* i = objmgr.GetMItem((*itr)->item);
            data << uint32(i->GetUInt32Value(OBJECT_FIELD_ENTRY));
        }
        else
        {
            data << uint32(0);
        }
        data << uint32(0);                        
        data << uint32(0);                        
        data << uint32(0);                        
        data << uint8(1);                         
        data << uint32(0xFFFFFFFF);               
        data << uint32(0);                        
        data << uint32(0);                        
        data << uint32((*itr)->money);            
        data << uint32((*itr)->COD);              
        data << uint32((*itr)->checked);          
        
        data << float(((*itr)->time - time(NULL)) / 3600);
    }
    SendPacket(&data);
}

extern char *fmtstring( char *format, ... );

uint32 GetItemGuidFromDisplayID ( uint32 displayID, Player* pl )
{
	
	

	

	uint8 i = 0;
	Item * srcitem;

	for (i = EQUIPMENT_SLOT_START; i < BANK_SLOT_BAG_END; i++)
	{
		srcitem = pl->GetItemBySlot(i);

		if (srcitem)
		{
			if (srcitem->GetItemProto()->DisplayInfoID == displayID)
			{
				break;
			}
		}
	}

    if( i >= BANK_SLOT_BAG_END )
	{
        QueryResult *result = sDatabase.Query( fmtstring("SELECT entry FROM items WHERE displayid='%u'", displayID) );

		if( !result )
		{
	        return -1;
		}

		uint32 id = (*result)[0].GetUInt32();
		return id;
	}

	return srcitem->GetItemProto()->ItemId;
}


extern char *GetInventoryImageFilefromObjectClass(uint32 classNum, uint32 subclassNum, uint32 type, uint32 DisplayID);

bool WorldSession::SendItemInfo( uint32 itemid, WorldPacket data )
{
    int i;
	Player* pl = GetPlayer();
	uint32 realID = GetItemGuidFromDisplayID(itemid, pl);
	char *itemInfo;
	bool resist_added = false;
	bool names_added = false;

	if (realID < 0)
	{
        Log::getSingleton( ).outError( "WORLD: Unknown item id 0x%.8X", realID );
        return false;
    }

    ItemPrototype *itemProto = objmgr.GetItemPrototype(realID);
    
	if(!itemProto)
    {
        Log::getSingleton( ).outError( "WORLD: Unknown item id 0x%.8X", realID );
        return false;
    }

	Log::getSingleton( ).outDebug( "WORLD: Real item id is %u. Name %s.", realID, itemProto->Name1.c_str() );

	data.Initialize(SMSG_ITEM_TEXT_QUERY_RESPONSE);
    data << itemid;

	itemInfo = (fmtstring("<HTML>\n<BODY>\n"));

	itemInfo = (fmtstring("%s<H1 align=\"left\">Name: %s</H1><BR/>", itemInfo, itemProto->Name1.c_str()));

	itemInfo = (fmtstring("%s<IMG src=\"Interface\\Icons\\%s\" align=\"right\"/><P align=\"left\">", itemInfo, GetInventoryImageFilefromObjectClass(itemProto->Class, itemProto->SubClass, itemProto->InventoryType, itemProto->DisplayInfoID)));
	itemInfo = (fmtstring("%s<BR/>", itemInfo));

    if (stricmp(itemProto->Name2.c_str(), "") && stricmp(itemProto->Name2.c_str(), " ") && stricmp(itemProto->Name2.c_str(), itemProto->Name1.c_str()))
    {
        itemInfo = (fmtstring("%s%s<BR/>", itemInfo, itemProto->Name2.c_str()));
		names_added = true;
    }

    if (stricmp(itemProto->Name3.c_str(), "") && stricmp(itemProto->Name2.c_str(), " ") && stricmp(itemProto->Name3.c_str(), itemProto->Name2.c_str()))
    {
        itemInfo = (fmtstring("%s%s<BR/>", itemInfo, itemProto->Name3.c_str()));
		names_added = true;
    }

    if (stricmp(itemProto->Name4.c_str(), "") && stricmp(itemProto->Name2.c_str(), " ") && stricmp(itemProto->Name4.c_str(), itemProto->Name3.c_str()))
    {
        itemInfo = (fmtstring("%s%s<BR/>", itemInfo, itemProto->Name4.c_str()));
		names_added = true;
    }

	if (names_added)
		itemInfo = (fmtstring("%s<BR/>", itemInfo)); 

    if (stricmp(itemProto->Description.c_str(), ""))
    {
		itemInfo = (fmtstring("%sDescription: %s<BR/>", itemInfo, itemProto->Description.c_str()));
		itemInfo = (fmtstring("%s<BR/>", itemInfo)); 
    }

	
	if (itemProto->Quality == ITEM_QUALITY_NORMAL)
		itemInfo = (fmtstring("%sThis is a normal item.<BR/><BR/>", itemInfo));
	else if (itemProto->Quality == ITEM_QUALITY_UNCOMMON)
		itemInfo = (fmtstring("%sThis is an uncommon item.<BR/><BR/>", itemInfo));
	else if (itemProto->Quality == ITEM_QUALITY_RARE)
		itemInfo = (fmtstring("%sThis is a rare item.<BR/><BR/>", itemInfo));
	else if (itemProto->Quality == ITEM_QUALITY_EPIC)
		itemInfo = (fmtstring("%sThis is an epic item.<BR/><BR/>", itemInfo));
	else if (itemProto->Quality == ITEM_QUALITY_LEGENDARY)
		itemInfo = (fmtstring("%sThis is a legendary item.<BR/><BR/>", itemInfo));

	if (itemProto->Bonding)
		itemInfo = (fmtstring("%sThis is a bonding item.<BR/>", itemInfo));

	itemInfo = (fmtstring("%sMaximum Durability: %u.<BR/>", itemInfo, itemProto->MaxDurability));


	for(i = 0; i < 5; i++)
	{
		if ( (itemProto->DamageMax[i] <= 0 || itemProto->DamageMax[i] <= 0)
			|| (itemProto->DamageMax[i] > 99999 || itemProto->DamageMax[i] > 99999) )
			continue; 
		
		switch (itemProto->DamageType[i])
		{
		case NORMAL_DAMAGE:
			itemInfo = (fmtstring("%sDoes %u to %u of normal damage.<BR/>", itemInfo, (uint32)itemProto->DamageMin[i], (uint32)itemProto->DamageMax[i]));
			break;
		case HOLY_DAMAGE:
			itemInfo = (fmtstring("%sadds %u to %u Holy damage.<BR/>", itemInfo, (uint32)itemProto->DamageMin[i], (uint32)itemProto->DamageMax[i]));
			break;
		case FIRE_DAMAGE:
			itemInfo = (fmtstring("%sadds %u to %u Fire damage.<BR/>", itemInfo, (uint32)itemProto->DamageMin[i], (uint32)itemProto->DamageMax[i]));
			break;
		case NATURE_DAMAGE:
			itemInfo = (fmtstring("%sadds %u to %u Nature damage.<BR/>", itemInfo, (uint32)itemProto->DamageMin[i], (uint32)itemProto->DamageMax[i]));
			break;
		case FROST_DAMAGE:
			itemInfo = (fmtstring("%sadds %u to %u Frost damage.<BR/>", itemInfo, (uint32)itemProto->DamageMin[i], (uint32)itemProto->DamageMax[i]));
			break;
		case SHADOW_DAMAGE:
			itemInfo = (fmtstring("%sadds %u to %u Shadow damage.<BR/>", itemInfo, (uint32)itemProto->DamageMin[i], (uint32)itemProto->DamageMax[i]));
			break;
		case ARCANE_DAMAGE:
			itemInfo = (fmtstring("%sadds %u to %u Arcane damage.<BR/>", itemInfo, (uint32)itemProto->DamageMin[i], (uint32)itemProto->DamageMax[i]));
			break;
		default: 
			break;
		}
	}

    itemInfo = (fmtstring("%sSell Price: %u.<BR/>", itemInfo, itemProto->SellPrice));
	itemInfo = (fmtstring("%s<BR/>", itemInfo)); 

    itemInfo = (fmtstring("%sLevel: %u.<BR/>", itemInfo, itemProto->ItemLevel));
	itemInfo = (fmtstring("%sRequired Character Level: %u.<BR/>", itemInfo, itemProto->RequiredLevel));
	itemInfo = (fmtstring("%s<BR/>", itemInfo)); 

    if (itemProto->ContainerSlots)
	{
		itemInfo = (fmtstring("%sThis item is a container, and will hold %u items.<BR/>", itemInfo, itemProto->ContainerSlots));
		itemInfo = (fmtstring("%s<BR/>", itemInfo)); 
	}

	
    
	if (itemProto->Armor > 0)
	{
		itemInfo = (fmtstring("%sArmor Bonus: %u.<BR/>", itemInfo, itemProto->Armor));
		itemInfo = (fmtstring("%s<BR/>", itemInfo)); 
	}

	if (itemProto->HolyRes > 0)
	{
		itemInfo = (fmtstring("%sHoly Resistance Bonus: %u.<BR/>", itemInfo, itemProto->HolyRes));
		resist_added = true;
	}

	if (itemProto->FireRes > 0)
	{
		itemInfo = (fmtstring("%sFire Resistance Bonus: %u.<BR/>", itemInfo, itemProto->FireRes));
		resist_added = true;
	}

	if (itemProto->NatureRes > 0)
	{
		itemInfo = (fmtstring("%sNature Resistance Bonus: %u.<BR/>", itemInfo, itemProto->NatureRes));
		resist_added = true;
	}

	if (itemProto->FrostRes > 0)
	{
		itemInfo = (fmtstring("%sFrost Resistance Bonus: %u.<BR/>", itemInfo, itemProto->FrostRes));
		resist_added = true;
	}

	if (itemProto->ShadowRes > 0)
	{
		itemInfo = (fmtstring("%sShadow Resistance Bonus: %u.<BR/>", itemInfo, itemProto->ShadowRes));
		resist_added = true;
	}

	if (itemProto->ArcaneRes > 0)
	{
		itemInfo = (fmtstring("%sArcane Resistance Bonus: %u.<BR/>", itemInfo, itemProto->ArcaneRes));
		resist_added = true;
	}

	if (resist_added)
		itemInfo = (fmtstring("%s<BR/>", itemInfo)); 
    
	itemInfo = (fmtstring("%sAttack Delay: %u.<BR/><BR/>", itemInfo, itemProto->Delay));

	itemInfo = (fmtstring("%s</P></BODY>\n</HTML>\n", itemInfo));

	data << itemInfo;
	data << uint32(0);
    SendPacket(&data);  

	

	return true;
}

void WorldSession::HandleItemTextQuery(WorldPacket & recv_data )
{
    WorldPacket data;
    uint32 mailguid;
    uint64 unk1;

    recv_data >> mailguid >> unk1;

    Player* pl = GetPlayer();
    
    Mail *itr;
    
    itr = pl->GetMail(mailguid);
    if(itr)   
    {
		Log::getSingleton().outDebug("We got mailguid: %d with unk: %d", mailguid, unk1);

        data.Initialize(SMSG_ITEM_TEXT_QUERY_RESPONSE);
        data << mailguid;
        data << itr->body.c_str();
        data << uint32(0);
        SendPacket(&data);
    }
    else
	{


        
		
		QueryResult *result = sDatabase.Query( fmtstring("SELECT * FROM item_pages WHERE id='%u'", mailguid) );

		if( result )
		{
			data.Initialize(SMSG_ITEM_TEXT_QUERY_RESPONSE);
			data << mailguid;

			uint32 nextpage = mailguid;

			while (nextpage)
			{
				data << (*result)[1].GetString();
				nextpage = (*result)[2].GetUInt32();
				data << nextpage;
			}

			data << uint32(0);
			SendPacket(&data); 
	        return;
		}
		
		
		if (!SendItemInfo( mailguid, data ))
		{
			data.Initialize(SMSG_ITEM_TEXT_QUERY_RESPONSE);
			data << mailguid;

			data << "There is no info for this item.";

			data << uint32(0);
			SendPacket(&data);  
		}
    }
}

void WorldSession::HandleMailCreateTextItem(WorldPacket & recv_data )
{
    uint32 unk1,unk2,mailid;

    recv_data >> unk1 >> unk2 >> mailid;

    Log::getSingleton().outString("HandleMailCreateTextItem unk1=%d,unk2=%d,mailid=%d",unk1,unk2,mailid);

    uint32 sbit2=5;
    bool   slotfree=false;
    WorldPacket Data;
    uint8 i,slot;

    Player* pl = GetPlayer();

    for(i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
    {
        if (GetPlayer()->GetItemBySlot(i) == NULL)
        {
            slot = i;
            slotfree=true;
            break;
        }
    }
    if (slotfree)
    {
        Item *item = new Item();
        
        item->Create(objmgr.GenerateLowGuid(HIGHGUID_ITEM), 889, GetPlayer());
        item->SetUInt32Value( ITEM_FIELD_ITEM_TEXT_ID , mailid );

        GetPlayer()->AddItemToSlot( slot, item );
		item->UpdateStats();

        Data.Initialize(SMSG_SEND_MAIL_RESULT);
        Data << uint32(mailid);
        Data << uint32(sbit2);
        Data << uint32(0);
        SendPacket(&Data);    
    }
    else
    { 
        Data.Initialize(SMSG_SEND_MAIL_RESULT);
        Data << uint32(mailid);
        Data << uint32(0);
        Data << uint32(1);
        SendPacket(&Data);   
    }
    
}

void WorldSession::HandleMsgQueryNextMailtime(WorldPacket & recv_data )
{
    
	WorldPacket Data;
	bool checkmail=false;
	Player *pl=GetPlayer();

	std::list<Mail*>::iterator itr;
    for (itr = pl->GetmailBegin(); itr != pl->GetmailEnd();itr++)
    {
		if(!(*itr)->checked){
             checkmail = true;
			 break;
		}
	}
	
    if ( checkmail )
	{
	    Data.Initialize(MSG_QUERY_NEXT_MAIL_TIME);
        Data << uint32(0); 
        SendPacket(&Data); 
	}
	else
	{
	    Data.Initialize(MSG_QUERY_NEXT_MAIL_TIME);
        Data << uint8(0x00);  
		Data << uint8(0xC0);
		Data << uint8(0xA8);
		Data << uint8(0xC7);
        SendPacket(&Data); 
	}

}

