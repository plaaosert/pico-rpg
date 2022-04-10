#include <malloc.h>

#define BASE_PLAYER_HP 100
#define BASE_PLAYER_MP 25
#define DEFAULT_MP_COLOUR ST7735_BLUE
#define STAMINA_MP_COLOUR ST7735_YELLOW
#define BASE_PLAYER_ATK 10
#define BASE_PLAYER_DEF 10
#define BASE_PLAYER_MAG 10
#define BASE_PLAYER_WIL 10

enum EquipmentSpecial {ES_NONE, ES_PISS_ONES_OWN_PANT, ES_THIS_IS_NOT_A_TEST};
enum SkillSpecial {SS_NONE, SS_INSTANT_KILL};
enum EquipSlot {HEAD, CHEST, GLOVES, BOOTS, WEAPON};
enum EquipType {
    ET_NOT_WEAPON, ET_DAGGER, ET_SWORD, ET_LONGSWORD, ET_AXE, ET_POLEARM, ET_HEAVY_ARMAMENT,
    ET_WAND, ET_ARCANE_STAFF, ET_NATURE_STAFF, ET_TALISMAN
};
enum AttackType {AT_PHYSICAL, AT_MAGICAL, AT_MULTI, AT_PURE};


typedef struct {
  char* name;
  char* desc;

  int hp;
  int max_hp;

  int mp;
  int max_mp;
  uint16_t mp_color;

  int xp;
  int level;
  int xp_to_next;

  int base_atk;
  int base_def;
  int base_mag;
  int base_wil;

  int atk;  // physical dmg output
  int def;  // physical dmg reduction
  int mag;  // magical dmg output
  int wil;  // magical dmg reduction

  int equipped[5];  // Head, Chest, Gloves, Boots, Weapon

  int* specials;  // all specials currently active from equipment

  int* innate_skills;  // Skills are learned by the character based on the type of weapon they currently hold.
  // Some skills can be retained no matter the weapon; those skills are here.

  int* all_skills;

  // 11 slots; first is for WEAPON and is a "dummy" for when the player has no weapon.
  // however, this is technically a "job" on its own, and has its own skills and level curve!
  int* weapon_type_levels;
} Character;


typedef struct {
	char* name;
	char* desc;

	// all additive
	int hp;
	int mp;
	int atk;
	int def;
	int mag;
	int wil;

    int type;
	int special;  // could be anything!
	int equip_slot;  // Head, Chest, Gloves, Boots, Weapon
} Equipment;


typedef struct {
    char* name;
    char* desc;

    int mp_cost;
    int power;
    int attack_type;

    int special;  // could be anything!
    int special_param_1;  // things like status effect chance, random bounds, etc.
    int special_param_2;
} Skill;


// Hopefully no enemy uses more than 8 skills. If they do, um, well, uh
int* make_skill_array(int skill0, int skill1, int skill2, int skill3, int skill4, int skill5, int skill6, int skill7) {
    int *skill_arr = malloc(8);

    skill_arr[0] = skill0;
    skill_arr[1] = skill1;
    skill_arr[2] = skill2;
    skill_arr[3] = skill3;
    skill_arr[4] = skill4;
    skill_arr[5] = skill5;
    skill_arr[6] = skill6;
    skill_arr[7] = skill7;

    return skill_arr;
}


int* make_equipment_array(int equip0, int equip1, int equip2, int equip3, int equip4) {
    int *equip_arr = malloc(8);

    equip_arr[0] = equip0;
    equip_arr[1] = equip1;
    equip_arr[2] = equip2;
    equip_arr[3] = equip3;

    return equip_arr;
}


int* make_empty_equips() {
    return make_equipment_array(0, 1, 2, 3, 4);
}


Character enemy_from_data(char* name, char* desc, int hp, int mp, int level, int atk, int def, int mag, int wil, int *equips, int *skills) {
    int s[] = {};

    Character enemy = {
        name, desc,
        hp, hp, mp, mp,
        DEFAULT_MP_COLOUR,
        0, level, 99999999,
        atk, def, mag, wil,
        atk, def, mag, wil,
        *s, *equips, *skills
    };

    return enemy;
}


// basic skills
// monster skills

// weapon skills
// ET_NOT_WEAPON, ET_DAGGER, ET_SWORD, ET_LONGSWORD, ET_AXE, ET_POLEARM, ET_HEAVY_ARMAMENT, ET_WAND, ET_ARCANE_STAFF, ET_NATURE_STAFF, ET_TALISMAN

// innate/learned skills
Skill skill_list[] = {
    // Basic
    {"Strike",         "Strike the target. Minimal damage.",                                  0, 20, AT_PHYSICAL, SS_NONE, -1, -1},
    {"Sparking",       "Cast basic magic. Minimal damage.",                                   2, 25, AT_MAGICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED BASIC SKILL",                                                  0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED BASIC SKILL",                                                  0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED BASIC SKILL",                                                  0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED BASIC SKILL",                                                  0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED BASIC SKILL",                                                  0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED BASIC SKILL",                                                  0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED BASIC SKILL",                                                  0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED BASIC SKILL",                                                  0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    // Monster (start index: 10)
    {"Flounder",       "Flounder around like the useless fish you are. Almost no damage.",    0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED MONSTER SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    // ET_NOT_WEAPON (WEAPON) (start index: 60)
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    // ET_DAGGER (70)
    {"Shadow Sneak",   "Stab a target in the back. Low chance to instantly kill non-boss targets.", 10, AT_PHYSICAL, SS_INSTANT_KILL, 4, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    // ET_SWORD (80)
    {"Multistrike",    "Execute a combination of strikes. Moderate damage.",                 10, 40, AT_PHYSICAL, SS_NONE, -1, -1},
    {"Steelcutter",    "A slice strong enough to sut steel. High damage.",                   35, 70, AT_PHYSICAL, SS_NONE, -1, -1},
    {"Dragonblade",    "Attack with a weapon infused with dragonblood. Huge damage.",        60, 120, AT_PHYSICAL, SS_NONE, -1, -1},
    {"Omnislash",      "Strike 1,000,000 times in a single moment. Overwhelming damage.",    100, 260, AT_PHYSICAL, SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    // ET_LONGSWORD (90)
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    // ET_AXE (100)
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    // ET_POLEARM (110)
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    // ET_HEAVY_ARMAMENT (120)
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    // ET_WAND (130)
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    // ET_ARCANE_STAFF (140)
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    // ET_NATURE_STAFF (150)
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    // ET_TALISMAN (160)
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    // Innate: weapon maximum level (170)
    {"Unused",         "UNUSED WEAPON MAX SKILL",                                            0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON MAX SKILL",                                            0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON MAX SKILL",                                            0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON MAX SKILL",                                            0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON MAX SKILL",                                            0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON MAX SKILL",                                            0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON MAX SKILL",                                            0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON MAX SKILL",                                            0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON MAX SKILL",                                            0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED WEAPON MAX SKILL",                                            0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    // Innate: skill books (182)
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},
    {"Unused",         "UNUSED INNATE SKILL",                                                0, 1, AT_PHYSICAL,  SS_NONE, -1, -1},

    // Innate: other (212)
    {"Final Skill",   "You shouldn't see this.", 0, AT_PHYSICAL, SS_NONE, -1, -1}
};


// ET_NOT_WEAPON, ET_DAGGER, ET_SWORD, ET_LONGSWORD, ET_AXE, ET_POLEARM, ET_HEAVY_ARMAMENT, ET_WAND, ET_ARCANE_STAFF, ET_NATURE_STAFF, ET_TALISMAN
int weapon_skill_levels[11][10] = {
    {1, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999},
    {1, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999},
    {1, 10, 25, 50, 100, 99999, 99999, 99999, 99999, 99999},
    {1, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999},
    {1, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999},
    {1, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999},
    {1, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999},
    {1, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999},
    {1, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999},
    {1, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999},
    {1, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999}
};


int weapon_skills[11][10] = {
    {0, 212, 212, 212, 212, 212, 212, 212, 212, 212},
    {0, 212, 212, 212, 212, 212, 212, 212, 212, 212},
    {0, 80, 81, 82, 83, 212, 212, 212, 212, 212},
    {0, 212, 212, 212, 212, 212, 212, 212, 212, 212},
    {0, 212, 212, 212, 212, 212, 212, 212, 212, 212},
    {0, 212, 212, 212, 212, 212, 212, 212, 212, 212},
    {0, 212, 212, 212, 212, 212, 212, 212, 212, 212},
    {1, 212, 212, 212, 212, 212, 212, 212, 212, 212},
    {1, 212, 212, 212, 212, 212, 212, 212, 212, 212},
    {1, 212, 212, 212, 212, 212, 212, 212, 212, 212},
    {1, 212, 212, 212, 212, 212, 212, 212, 212, 212}
};


void init_enemies(Character to_init[]) {
    to_init[0] = enemy_from_data(
        "G. Slime", "Weakest of the slime family. Its green colour indicates its preferred locale; the plains.",
        20, 5, 1,
        5, 2, 2, 3,
        make_empty_equips(),
        make_skill_array(0, 1, -1, -1, -1, -1, -1, -1)
    );

    to_init[1] = enemy_from_data(
        "Fish", "Level 1. Harmless.",
        10, 0, 1,
        1, 1, 1, 1,
        make_empty_equips(),
        make_skill_array(10, -1, -1, -1, -1, -1, -1, -1)
    );
}


Character enemy_list[100];
