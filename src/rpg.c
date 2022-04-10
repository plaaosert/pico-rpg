#include "pico/stdlib.h"
#include "pico/util/datetime.h"

#include <math.h>
#include <stdio.h>

#include "hardware/gpio.h"
#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"
#include "pico/time.h"

#include "functions.c"

#include "external/fonts.h"
#include "external/st7735.h"

const uint BUTTON1 = 20;
const uint BUTTON2 = 21;
const uint BUTTON3 = 22;

const uint16_t note_cols[8] = {
	ST7735_BLACK,
	ST7735_RED,
	ST7735_YELLOW,
	ST7735_GREEN,
	ST7735_CYAN,
	ST7735_BLUE,
	ST7735_MAGENTA,
	ST7735_WHITE
};

const float note_freqs[8] = {
	// 440, 493.88, (A4, B4)
	523.25, 587.33, 659.25, 698.46, 783.99, 880,
	987.77, 1046.50
};

// https://forums.raspberrypi.com/viewtopic.php?t=302960
uint32_t rnd() {
  int k, random=0;
  volatile uint32_t *rnd_reg=(uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);
  
  for(k=0; k<32; k++){
  
  random = random << 1;
  random = random + (0x00000001 & (*rnd_reg));

  }
  return random;
}

int secs_to_us(float secs) {
	return round(secs * 1000000);
}

void rest(int time_us) {
	sleep_us(time_us);
}

void play_note(int pitch, int time_us) {
	if (pitch != -1) {
		for (int i=0; i<(time_us / ((float)1000000 / note_freqs[pitch])); i++) {
			gpio_put(18, 1);
			sleep_us((float)1000000 / note_freqs[pitch]);
			gpio_put(18, 0);
			sleep_us((float)1000000 / note_freqs[pitch]);
		}
	} else {
		rest(time_us);
	}

	gpio_put(18, 0);
}

void play_note_sequence(int *notes, int *times_us, int num_notes) {
	for (int i=0; i<num_notes; i++) {
		play_note(notes[i], times_us[i]);
	}
}

void play_note_sequence_with_fills(int *notes, int *times_us, int num_notes) {
	for (int i=0; i<num_notes; i++) {
		if (notes[i] != -1) {
			ST7735_FillScreen(note_cols[notes[i] % 8]);
		}
		play_note(notes[i], times_us[i]);
	}
}

void setup() {
	stdio_init_all();
	setvbuf ( stdout , NULL , _IONBF , 0 );
	sleep_ms(1000);
	ST7735_Init();
	ST7735_DrawImage(0, 0, 80, 160, plaao_logo);
	sleep_ms(1000);

	const uint LED_PIN = 0;
	const uint BUTTON1 = 20;
	const uint BUTTON2 = 21;
	const uint BUTTON3 = 22;

	for (int i=18; i<28; i++) {
		gpio_init(i);

		if (i == 20 || i == 21 || i == 22) {
			gpio_set_dir(i, GPIO_IN);
		} else {
			gpio_set_dir(i, GPIO_OUT);
		}
	}

	int notes[] = {0, 2, 4, -1, 0, 1, 2, 3, 4, 5, 6, 7};
	int times[] = {100000, 100000, 100000, 250000, 20000, 20000, 20000, 20000, 20000, 20000, 20000, 50000};

	play_note_sequence_with_fills(notes, times, 13);

	ST7735_FillScreen(ST7735_BLACK);
	/*
	ST7735_WriteString(12, 6, Font4x6, "( )*+,-./0123456789:;<=>?@", ST7735_CYAN, ST7735_BLACK);
	ST7735_WriteString(12, 14, Font4x6, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", ST7735_CYAN, ST7735_BLACK);
	ST7735_WriteString(12, 22, Font4x6, "abcdefghijklmnopqrstuvwxyz", ST7735_CYAN, ST7735_BLACK);
	ST7735_WriteString(12, 30, Font4x6, "!\"#$%&'[\\]^_`{|}~", ST7735_CYAN, ST7735_BLACK);
	*/

	sleep_ms(400);

	init_enemies(enemy_list);

	Character player = make_character("plaao");
	Character enemy = enemy_list[0];

	int selected_option = 0;
	bool changed_option = true;
	bool button1_down = false;
	bool button2_down = false;
	int last_mp_gain = get_absolute_time();

	int option_locations_x[] = {
		8, 8, 8, 8
	};

	int option_locations_y[] = {
		6, 14, 22, 30
	};

	while (true) {
		if (gpio_get(BUTTON3) == 0) {
			int notes[] = {0, 1, 2, 3, 4, 5, 6, 7};
			int times[] = {2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500};

			play_note_sequence(notes, times, 8);

			ST7735_FillScreen(ST7735_BLACK);

			char str_write[80];
			Skill skill_selected = skill_list[selected_option + 80];

			sprintf(str_write, "%s       ", skill_selected.name);
			ST7735_WriteString(8, 6, Font4x6, str_write, ST7735_YELLOW, ST7735_BLACK);

			sprintf(str_write, "%d MP      ", skill_selected.mp_cost);
			ST7735_WriteString(8, 14, Font4x6, str_write, ST7735_CYAN, ST7735_BLACK);

			sprintf(str_write, "%s", skill_selected.desc);
			write_text_linewrap(8, 30, str_write, 80, ST7735_CYAN);

			while (true) {
				if (gpio_get(BUTTON1) == 0) {
					button1_down = true;
					break;
				} else if (gpio_get(BUTTON2) == 0) {
					button2_down = true;
					break;
				}

				sleep_ms(25);
			}

			ST7735_FillScreen(ST7735_BLACK);
			changed_option = true;
			last_mp_gain -= 200000;
		}

		// change selection
		if (gpio_get(BUTTON1) == 0 && !button1_down) {
			selected_option = (selected_option + 1) % 4;
			changed_option = true;

			int notes[] = {0, 7};
			int times[] = {2000, 2000};
			play_note_sequence(notes, times, 2);

			button1_down = true;
		} else if (gpio_get(BUTTON1) == 1) {
			button1_down = false;
		}

		// select
		if (gpio_get(BUTTON2) == 0 && !button2_down) {
			char dmg_string[20];
			int mp_cost = 10 * selected_option;

			if (player.mp < mp_cost) {
				int notes[] = {0, -1, 0};
				int times[] = {100000, 100000, 100000};
				play_note_sequence(notes, times, 3);
			} else {
				play_note(selected_option, 150000);
				if (selected_option != 2) {
					int dmg = ((float)rnd() / (float)0xffffffff) * (50 * (selected_option + 1));
					sprintf(dmg_string, "%d damage!    ", dmg);
					enemy.hp -= dmg;
					player.mp -= (10 * selected_option);

					ST7735_WriteString(96, 72, Font4x6, dmg_string, ST7735_RED, ST7735_BLACK);
				} else {
					int dmg = 250 + ((float)rnd() / (float)0xffffffff) * 750;
					sprintf(dmg_string, "%d heal!     ", dmg);
					player.hp += dmg;
					enemy.hp = enemy.max_hp;
					player.mp -= (20 * selected_option);

					ST7735_WriteString(96, 72, Font4x6, dmg_string, ST7735_GREEN, ST7735_BLACK);
				}

				if (enemy.hp < 0) {
					enemy.hp = 0;
				} else if (player.hp > player.max_hp) {
					player.hp = player.max_hp;
				}

				if (player.mp < 0) {
					player.mp = 0;
				}

				last_mp_gain -= 200000;
			}
		} else if (gpio_get(BUTTON2) == 1) {
			button2_down = false;
		}

		if (changed_option) {
			char str_write[20];
			changed_option = false;

			Skill skill_selected = skill_list[selected_option + 80];

			sprintf(str_write, "%d MP     ", skill_selected.mp_cost);
			ST7735_WriteString(48, 38, Font4x6, str_write, ST7735_CYAN, ST7735_BLACK);
			
			for (int i=0; i<4; i++) {
				sprintf(str_write, "%s%s ", (i == selected_option ? ">" : ""), skill_list[i + 80].name);
				ST7735_WriteString(option_locations_x[i], option_locations_y[i], Font4x6, str_write, (i == selected_option ? ST7735_YELLOW : ST7735_CYAN), ST7735_BLACK);
			}
		}

		//char str_write[40];
		//sprintf(str_write, "%d", get_absolute_time());
		//ST7735_WriteString(96, 32, Font4x6, str_write, ST7735_WHITE, ST7735_BLACK);

		if (get_absolute_time() - last_mp_gain > 200000) {
			player.mp += 1;
			if (player.mp >= player.max_mp) {
				player.max_mp = player.mp;
			}

			last_mp_gain += 200000;
			ST7735_WriteString(8, 42, Font4x6, player.name, ST7735_YELLOW, ST7735_BLACK);
			render_health_bar(player, 4, 51, 72, 4, ST7735_BLACK);
			render_mana_bar(player, 4, 67, 72, 4, ST7735_BLACK);

			ST7735_WriteString(88, 8, Font4x6, enemy.name, ST7735_YELLOW, ST7735_BLACK);
			render_enemy_health_bar(enemy, 84, 16, 54, 4, ST7735_BLACK);
		}
	}
}

int main() {
	setup();
}