/* 
 * Copyright (C) 2005,2006 MaNGOS <http://www.mangosproject.org/>
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
#ifndef MANGOS_SPELLAURAS_H
#define MANGOS_SPELLAURAS_H

#include "Spell.h"

#define MAX_AURAS 56
#define MAX_NEGATIVE_AURAS 32

enum AURA_FLAGS
{
    AFLAG_EMPTY = 0x0,
    AFLAG_SET = 0x9
};

//m_schoolAbsorb
enum DAMAGE_ABSORB_TYPE
{
    ALL_DAMAGE_ABSORB = -2,
    ONLY_MAGIC_ABSORB  = -1,
};

enum MOD_TYPES
{
    SPELL_AURA_NONE = 0,
    SPELL_AURA_BIND_SIGHT = 1,
    SPELL_AURA_MOD_POSSESS = 2,
    SPELL_AURA_PERIODIC_DAMAGE = 3,
    SPELL_AURA_DUMMY = 4,
    SPELL_AURA_MOD_CONFUSE = 5,
    SPELL_AURA_MOD_CHARM = 6,
    SPELL_AURA_MOD_FEAR = 7,
    SPELL_AURA_PERIODIC_HEAL = 8,
    SPELL_AURA_MOD_ATTACKSPEED = 9,
    SPELL_AURA_MOD_THREAT = 10,
    SPELL_AURA_MOD_TAUNT = 11,
    SPELL_AURA_MOD_STUN = 12,
    SPELL_AURA_MOD_DAMAGE_DONE = 13,
    SPELL_AURA_MOD_DAMAGE_TAKEN = 14,
    SPELL_AURA_DAMAGE_SHIELD = 15,
    SPELL_AURA_MOD_STEALTH = 16,
    SPELL_AURA_MOD_DETECT = 17,
    SPELL_AURA_MOD_INVISIBILITY = 18,
    SPELL_AURA_MOD_INVISIBILITY_DETECTION = 19,
    //missing 20,21
    SPELL_AURA_MOD_RESISTANCE = 22,
    SPELL_AURA_PERIODIC_TRIGGER_SPELL = 23,
    SPELL_AURA_PERIODIC_ENERGIZE = 24,
    SPELL_AURA_MOD_PACIFY = 25,
    SPELL_AURA_MOD_ROOT = 26,
    SPELL_AURA_MOD_SILENCE = 27,
    SPELL_AURA_REFLECT_SPELLS = 28,
    SPELL_AURA_MOD_STAT = 29,
    SPELL_AURA_MOD_SKILL = 30,
    SPELL_AURA_MOD_INCREASE_SPEED = 31,
    SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED = 32,
    SPELL_AURA_MOD_DECREASE_SPEED = 33,
    SPELL_AURA_MOD_INCREASE_HEALTH = 34,
    SPELL_AURA_MOD_INCREASE_ENERGY = 35,
    SPELL_AURA_MOD_SHAPESHIFT = 36,
    SPELL_AURA_EFFECT_IMMUNITY = 37,
    SPELL_AURA_STATE_IMMUNITY = 38,
    SPELL_AURA_SCHOOL_IMMUNITY = 39,
    SPELL_AURA_DAMAGE_IMMUNITY = 40,
    SPELL_AURA_DISPEL_IMMUNITY = 41,
    SPELL_AURA_PROC_TRIGGER_SPELL = 42,
    SPELL_AURA_PROC_TRIGGER_DAMAGE = 43,
    SPELL_AURA_TRACK_CREATURES = 44,
    SPELL_AURA_TRACK_RESOURCES = 45,
    SPELL_AURA_MOD_PARRY_SKILL = 46,
    SPELL_AURA_MOD_PARRY_PERCENT = 47,
    SPELL_AURA_MOD_DODGE_SKILL = 48,
    SPELL_AURA_MOD_DODGE_PERCENT = 49,
    SPELL_AURA_MOD_BLOCK_SKILL = 50,
    SPELL_AURA_MOD_BLOCK_PERCENT = 51,
    SPELL_AURA_MOD_CRIT_PERCENT = 52,
    SPELL_AURA_PERIODIC_LEECH = 53,
    SPELL_AURA_MOD_HIT_CHANCE = 54,
    SPELL_AURA_MOD_SPELL_HIT_CHANCE = 55,
    SPELL_AURA_TRANSFORM = 56,
    SPELL_AURA_MOD_SPELL_CRIT_CHANCE = 57,
    SPELL_AURA_MOD_INCREASE_SWIM_SPEED = 58,
    SPELL_AURA_MOD_DAMAGE_DONE_CREATURE = 59,
    SPELL_AURA_MOD_PACIFY_SILENCE = 60,
    SPELL_AURA_MOD_SCALE = 61,
    SPELL_AURA_PERIODIC_HEALTH_FUNNEL = 62,
    SPELL_AURA_PERIODIC_MANA_FUNNEL = 63,
    SPELL_AURA_PERIODIC_MANA_LEECH = 64,
    SPELL_AURA_MOD_CASTING_SPEED = 65,
    SPELL_AURA_FEIGN_DEATH = 66,
    SPELL_AURA_MOD_DISARM = 67,
    SPELL_AURA_MOD_STALKED = 68,
    SPELL_AURA_SCHOOL_ABSORB = 69,
    SPELL_AURA_EXTRA_ATTACKS = 70,
    SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL = 71,
    SPELL_AURA_MOD_POWER_COST = 72,
    SPELL_AURA_MOD_POWER_COST_SCHOOL = 73,
    SPELL_AURA_REFLECT_SPELLS_SCHOOL = 74,
    SPELL_AURA_MOD_LANGUAGE = 75,
    SPELL_AURA_FAR_SIGHT = 76,
    SPELL_AURA_MECHANIC_IMMUNITY = 77,
    SPELL_AURA_MOUNTED = 78,
    SPELL_AURA_MOD_DAMAGE_PERCENT_DONE = 79,
    SPELL_AURA_MOD_PERCENT_STAT = 80,
    SPELL_AURA_SPLIT_DAMAGE = 81,
    SPELL_AURA_WATER_BREATHING = 82,
    SPELL_AURA_MOD_BASE_RESISTANCE = 83,
    SPELL_AURA_MOD_REGEN = 84,
    SPELL_AURA_MOD_POWER_REGEN = 85,
    SPELL_AURA_CHANNEL_DEATH_ITEM = 86,
    SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN = 87,
    SPELL_AURA_MOD_PERCENT_REGEN = 88,
    SPELL_AURA_PERIODIC_DAMAGE_PERCENT = 89,
    SPELL_AURA_MOD_RESIST_CHANCE = 90,
    SPELL_AURA_MOD_DETECT_RANGE = 91,
    SPELL_AURA_PREVENTS_FLEEING = 92,
    SPELL_AURA_MOD_UNATTACKABLE = 93,
    SPELL_AURA_INTERRUPT_REGEN = 94,
    SPELL_AURA_GHOST = 95,
    SPELL_AURA_SPELL_MAGNET = 96,
    SPELL_AURA_MANA_SHIELD = 97,
    SPELL_AURA_MOD_SKILL_TALENT = 98,
    SPELL_AURA_MOD_ATTACK_POWER = 99,
    SPELL_AURA_AURAS_VISIBLE = 100,
    SPELL_AURA_MOD_RESISTANCE_PCT = 101,
    SPELL_AURA_MOD_CREATURE_ATTACK_POWER = 102,
    SPELL_AURA_MOD_TOTAL_THREAT = 103,
    SPELL_AURA_WATER_WALK = 104,
    SPELL_AURA_FEATHER_FALL = 105,
    SPELL_AURA_HOVER = 106,
    SPELL_AURA_ADD_FLAT_MODIFIER = 107,
    SPELL_AURA_ADD_PCT_MODIFIER = 108,
    SPELL_AURA_ADD_TARGET_TRIGGER = 109,
    SPELL_AURA_MOD_POWER_REGEN_PERCENT = 110,
    SPELL_AURA_ADD_CASTER_HIT_TRIGGER = 111,
    SPELL_AURA_OVERRIDE_CLASS_SCRIPTS = 112,
    SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN = 113,
    SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN_PCT = 114,
    SPELL_AURA_MOD_HEALING = 115,
    SPELL_AURA_IGNORE_REGEN_INTERRUPT = 116,
    SPELL_AURA_MOD_MECHANIC_RESISTANCE = 117,
    SPELL_AURA_MOD_HEALING_PCT = 118,
    SPELL_AURA_SHARE_PET_TRACKING = 119,
    SPELL_AURA_UNTRACKABLE = 120,
    SPELL_AURA_EMPATHY = 121,
    SPELL_AURA_MOD_OFFHAND_DAMAGE_PCT = 122,
    SPELL_AURA_MOD_POWER_COST_PCT = 123,
    SPELL_AURA_MOD_RANGED_ATTACK_POWER = 124,
    SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN = 125,
    SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN_PCT = 126,
    SPELL_AURA_RANGED_ATTACK_POWER_ATTACKER_BONUS = 127,
    SPELL_AURA_MOD_POSSESS_PET = 128,
    SPELL_AURA_MOD_INCREASE_SPEED_ALWAYS = 129,
    SPELL_AURA_MOD_MOUNTED_SPEED_ALWAYS = 130,
    SPELL_AURA_MOD_CREATURE_RANGED_ATTACK_POWER = 131,
    SPELL_AURA_MOD_INCREASE_ENERGY_PERCENT = 132,
    SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT = 133,
    SPELL_AURA_MOD_MANA_REGEN_INTERRUPT = 134,
    SPELL_AURA_MOD_HEALING_DONE = 135,
    SPELL_AURA_MOD_HEALING_DONE_PERCENT = 136,
    SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE = 137,
    SPELL_AURA_MOD_HASTE = 138,
    SPELL_AURA_FORCE_REACTION = 139,
    SPELL_AURA_MOD_RANGED_HASTE = 140,
    SPELL_AURA_MOD_RANGED_AMMO_HASTE = 141,
    SPELL_AURA_MOD_BASE_RESISTANCE_PCT = 142,
    SPELL_AURA_MOD_RESISTANCE_EXCLUSIVE = 143,
    SPELL_AURA_SAFE_FALL = 144,
    SPELL_AURA_CHARISMA = 145,
    SPELL_AURA_PERSUADED = 146,
    SPELL_AURA_ADD_CREATURE_IMMUNITY = 147,
    SPELL_AURA_RETAIN_COMBO_POINTS = 148,
    SPELL_AURA_RESIST_PUSHBACK  =   149 ,                   //	Resist Pushback
    SPELL_AURA_MOD_SHIELD_BLOCK =   150 ,                   //	Mod Shield Block %
    SPELL_AURA_TRACK_STEALTHED  =   151 ,                   //	Track Stealthed
    SPELL_AURA_MOD_DETECTED_RANGE   =   152 ,               //	Mod Detected Range
    SPELL_AURA_SPLIT_DAMAGE_FLAT    =   153 ,               //	Split Damage Flat
    SPELL_AURA_MOD_STEALTH_LEVEL    =   154 ,               //	Stealth Level Modifier
    SPELL_AURA_MOD_WATER_BREATHING  =   155 ,               //	Mod Water Breathing
    SPELL_AURA_MOD_REPUTATION_ADJUST    =   156 ,           //	Mod Reputation Gain
    SPELL_AURA_PET_DAMAGE_MULTI =   157 ,                   //	Mod Pet Damage

    TOTAL_AURAS=158

};

struct DamageManaShield
{
    uint32 m_spellId;
    uint32 m_modType;
    int32 m_schoolAbsorb;
    uint32 m_totalAbsorb;
    uint32 m_currAbsorb;
};

struct Modifier
{
    uint8 m_auraname;
    int32 m_amount;
    int32 m_miscvalue;
    uint32 m_miscvalue2;
    uint32 periodictime;
};

class Unit;
struct SpellEntry;
struct ProcTriggerSpell;

class Aura
{
    public:
        typedef std::list<Modifier*> ModList;

        //aura handlers
        void HandleNULL(bool Apply);
        void HandlePeriodicDamage(bool apply);
        void HandlePeriodicHeal(bool apply);
        void HandleAuraWaterWalk(bool Apply);
        void HandleAuraFeatherFall(bool Apply);
        void HandleAuraModStun(bool Apply);
        void HandleAuraModRangedAttackPower(bool Apply);
        void HandleAuraModIncreaseSpeedAlways(bool Apply);
        void HandleAuraModIncreaseEnergyPercent(bool Apply);
        void HandleAuraModIncreaseHealthPercent(bool Apply);
        void HandlePeriodicTriggerSpell(bool apply);
        void HandleAuraModResistanceExclusive(bool Apply);
        void HandleAuraSafeFall(bool Apply);
        void HandleAuraDamageShield(bool Apply);
        void HandleModStealth(bool Apply);
        void HandleAuraModResistance(bool Apply);
        void HandleAuraModRoot(bool Apply);
        void HandleAuraModSilence(bool Apply);
        void HandleAuraModStat(bool Apply);
        void HandleAuraModIncreaseSpeed(bool Apply);
        void HandleAuraModIncreaseMountedSpeed(bool Apply);
        void HandleAuraModDecreaseSpeed(bool Apply);
        void HandleAuraModIncreaseHealth(bool Apply);
        void HandleAuraModIncreaseEnergy(bool Apply);
        void HandleAuraModShapeshift(bool Apply);
        void HandleAuraModEffectImmunity(bool Apply);
        void HandleAuraModStateImmunity(bool Apply);
        void HandleAuraModSchoolImmunity(bool Apply);
        void HandleAuraModDmgImmunity(bool Apply);
        void HandleAuraModDispelImmunity(bool Apply);
        void HandleAuraProcTriggerSpell(bool Apply);
        void HandleAuraProcTriggerDamage(bool Apply);
        void HandleAuraTracCreatures(bool Apply);
        void HandleAuraTracResources(bool Apply);
        void HandleAuraModParryPercent(bool Apply);
        void HandleAuraModDodgePercent(bool Apply);
        void HandleAuraModBlockPercent(bool Apply);
        void HandleAuraModCritPercent(bool Apply);
        void HandleAuraModScale(bool Apply);
        void HandleAuraMounted(bool Apply);
        void HandleWaterBreathing(bool apply);
        void HandleModRegen(bool apply);
        void HandleModPowerRegen(bool apply);
        void HandleAuraModAttackPower(bool Apply);
        void HandleAuraTransform(bool Apply);
        void HandleAuraManaShield(bool Apply);
        void HandleAuraSchoolAbsorb(bool Apply);
        void HandleReflectSpellsSchool(bool Apply);
        void HandleAuraModSkill(bool Apply);
        void HandleModDamagePercentDone(bool Apply);
        void HandleModPercentStat(bool Apply);
        void HandleModResistancePercent(bool Apply);

        Aura() : m_spellId(0), m_effIndex(0), m_caster(NULL), m_target(NULL), m_duration(0), m_auraSlot(0), m_positive(false), m_permanent(false), m_isPeriodic(false) {}
        Aura(uint32 spellid, uint32 eff, Unit *caster, Unit *target);
        Aura(uint32 spellid, uint32 eff, int32 duration, Unit *caster, Unit *target);
        Aura(SpellEntry* spellproto, uint32 eff, int32 duration, Unit *caster, Unit *target);

        void SetModifier(uint8 t, int32 a, uint32 pt, int32 miscValue, uint32 miscValue2);
        void SetModifier(Modifier* mod) {m_modifier=mod;}
        Modifier* GetModifier() {return m_modifier;}

        SpellEntry* GetSpellProto() const { return sSpellStore.LookupEntry( m_spellId ); }
        uint32 GetId() const{ return m_spellId; }
        uint32 GetEffIndex() const{ return m_effIndex; }
        void SetEffIndex(uint32 eff) { m_effIndex = eff; }
        int32 GetDuration() const { return m_duration; }
        void SetDuration(int32 duration) { m_duration = duration; }

        Unit* GetCaster() const { return m_caster; }
        Unit* GetTarget() const { return m_target; }
        void SetCaster(Unit* caster) { m_caster = caster; }
        void SetTarget(Unit* target) { m_target = target; }

        uint8 GetAuraSlot() const { return m_auraSlot; }
        void SetAuraSlot(uint8 slot) { m_auraSlot = slot; }

        bool IsPositive() { return m_positive; }
        void SetNegative() { m_positive = false; }
        void SetPositive() { m_positive = true; }

        bool IsPermanent() { return m_permanent; }
        void SetPermanent(bool value) { m_permanent = value; }

        void Update(uint32 diff);
        void ApplyModifier(bool Apply);

        void _AddAura();
        void _RemoveAura();
        uint32 CalculateDamage(SpellEntry* spellproto, uint8 i);

    private:

        Modifier *m_modifier;
        uint32 m_spellId;
        uint32 m_effIndex;
        //SpellEntry *m_spellProto;
        Unit* m_caster;
        Unit* m_target;
        int32 m_duration;

        uint8 m_auraSlot;

        bool m_positive;
        bool m_permanent;
        bool m_isPeriodic;
        uint32 m_periodicTimer;
        uint32 m_PeriodicEventId;
};

typedef void(Aura::*pAuraHandler)(bool Apply);
#endif
