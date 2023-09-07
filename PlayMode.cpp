#include "PlayMode.hpp"

#include "data_path.hpp"
//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include "Block.hpp"
#include "Load.hpp"
#include <random>

#include <stdio.h>
#include <bitset>

Load < Block > player_block(LoadTagDefault, []() -> Block * {
	return new Block(data_path("assets/eye_idle.png"));
});

Load < Block > player_block_up(LoadTagDefault, []() -> Block * {
	return new Block(data_path("assets/eye_up.png"));
});

Load < Block > player_block_down(LoadTagDefault, []() -> Block * {
	return new Block(data_path("assets/eye_down.png"));
});

Load < Block > player_block_left(LoadTagDefault, []() -> Block * {
	return new Block(data_path("assets/eye_left.png"));
});

Load < Block > player_block_right(LoadTagDefault, []() -> Block * {
	return new Block(data_path("assets/eye_right.png"));
});

Load < Block > player_block_left_up(LoadTagDefault, []() -> Block * {
	return new Block(data_path("assets/eye_left_up.png"));
});

Load < Block > player_block_left_down(LoadTagDefault, []() -> Block * {
	return new Block(data_path("assets/eye_left_down.png"));
});

Load < Block > player_block_right_up(LoadTagDefault, []() -> Block * {
	return new Block(data_path("assets/eye_right_up.png"));
});

Load < Block > player_block_right_down(LoadTagDefault, []() -> Block * {
	return new Block(data_path("assets/eye_right_down.png"));
});

Load < Block > background_block_1(LoadTagDefault, []() -> Block * {
	return new Block(data_path("assets/back_1.png"));
});

Load < Block > background_block_2(LoadTagDefault, []() -> Block * {
	return new Block(data_path("assets/back_2.png"));
});

Load < Block > goal_block(LoadTagDefault, []() -> Block * {
	return new Block(data_path("assets/goal.png"));
});

Load < Block > mask_block(LoadTagDefault, []() -> Block * {
	return new Block(data_path("assets/mask.png"));
});

Load < Block > enemy_block(LoadTagDefault, []() -> Block * {
	return new Block(data_path("assets/enemy.png"));
});

void set_bit0(std::array< uint8_t, 8 > &bit0, std::vector< uint8_t > sprites) {
	uint8_t i = 0;
	while (i < sprites.size()) {
		bit0[i] = sprites[i];
		i += 1;
	}
};

void set_bit1(std::array< uint8_t, 8 > &bit1) {
	bit1 = {
		0b0, 0b0, 0b0, 0b0,
		0b0, 0b0, 0b0, 0b0
	};
};

PlayMode::PlayMode() {
	// Tile/Sprite Configurations
	// setting the background for the game
	Block background_setting_1 = (*background_block_1);
	Block background_setting_2 = (*background_block_2);
	set_bit0(ppu.tile_table[0].bit0, background_setting_1.sprites);
	set_bit1(ppu.tile_table[0].bit1);
	set_bit0(ppu.tile_table[1].bit0, background_setting_2.sprites);
	set_bit1(ppu.tile_table[1].bit1);
	ppu.palette_table[0] = *(background_setting_1.palette);
	
	// setting the bits for the player
	Block player_setting = (*player_block);
	set_bit0(ppu.tile_table[32].bit0, player_setting.sprites);
	set_bit1(ppu.tile_table[32].bit1);
	ppu.palette_table[7] = *(player_setting.palette);

	// setting the bits for the goal
	Block goal_setting = (*goal_block);
	set_bit0(ppu.tile_table[16].bit0, goal_setting.sprites);
	set_bit1(ppu.tile_table[16].bit1);
	ppu.palette_table[2] = *(goal_setting.palette);

	// setting the bits for the mask
	Block mask_setting = (*mask_block);
	set_bit0(ppu.tile_table[17].bit0, mask_setting.sprites);
	set_bit1(ppu.tile_table[17].bit1);
	ppu.palette_table[3] = *(mask_setting.palette);

	// setting the bits for the enemies:
	Block enemy_setting = (*enemy_block);
	set_bit0(ppu.tile_table[33].bit0, enemy_setting.sprites);
	set_bit1(ppu.tile_table[33].bit1);
	ppu.palette_table[6] = *(enemy_setting.palette);

	// enemy speed set up
	for (size_t i = 0; i < MAXENEMY; i++) {
		int x = rand() % MAXENEMY;
		int y = rand() % MAXENEMY;
		enemy_direc[i].x = (1.0f - (x % 2) * 2) * (x / 4.0f);
		enemy_direc[i].y = (1.0f - (x % 2) * 2) * (y / 4.0f);
	}
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.pressed = false;
			return true;
		}
	}

	return false;
}

void PlayMode::update(float elapsed) {
	// player movements
	constexpr float PlayerSpeed = 50.0f;
	if (left.pressed) player_at.x -= PlayerSpeed * elapsed;
	if (right.pressed) player_at.x += PlayerSpeed * elapsed;
	if (down.pressed) player_at.y -= PlayerSpeed * elapsed;
	if (up.pressed) player_at.y += PlayerSpeed * elapsed;

	// player direction, controls sprites accordingly
	bool up_direc = false;
	bool down_direc = false;
	bool left_direc = false;
	bool right_direc = false;
	if (left.pressed && !right.pressed) {
		left_direc = true;
	} else if (right.pressed && !left.pressed) {
		right_direc = true;
	}
	if (up.pressed && !down.pressed) {
		up_direc = true;
	} else if (down.pressed && !up.pressed) {
		down_direc = true;
	}
	bool horizontal = left_direc || right_direc;
	bool vertical = up_direc || down_direc;
	if (!horizontal && !vertical) {
			set_bit0(ppu.tile_table[32].bit0, player_block->sprites);
	} else if (!horizontal && vertical) {
		if (up_direc) {
			set_bit0(ppu.tile_table[32].bit0, player_block_up->sprites);
		} else {
			set_bit0(ppu.tile_table[32].bit0, player_block_down->sprites);
		}
	} else if (horizontal && !vertical) {
		if (left_direc) {
			set_bit0(ppu.tile_table[32].bit0, player_block_right->sprites);
		} else {
			set_bit0(ppu.tile_table[32].bit0, player_block_left->sprites);
		}
	} else {
		if (left_direc && up_direc) {
			set_bit0(ppu.tile_table[32].bit0, player_block_right_up->sprites);
		} else if (left_direc && down_direc) {
			set_bit0(ppu.tile_table[32].bit0, player_block_right_down->sprites);
		} else if (right_direc && up_direc) {
			set_bit0(ppu.tile_table[32].bit0, player_block_left_up->sprites);
		} else if (right_direc && down_direc) {
			set_bit0(ppu.tile_table[32].bit0, player_block_left_down->sprites);
		}
	}

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;

	// enemy movements
	float EnemySpeed = 25.0f + (difficulty / 10) * 5.0f;
	for (size_t i = 0; i < enemy_at.size(); i++) {
		if (i % 2 == 1) {
			enemy_at[i].x += EnemySpeed * enemy_direc[i].x * elapsed;
		} else {
			enemy_at[i].x -= EnemySpeed * enemy_direc[i].x * elapsed;
		}
		if ((i / 2) % 2 == 1) {
			enemy_at[i].y += EnemySpeed * enemy_direc[i].y * elapsed;
		} else {
			enemy_at[i].y -= EnemySpeed * enemy_direc[i].y * elapsed;
		}
	}
	
	// enemy check
	hit = false;
	for (size_t i = 0; i < MAXENEMY; i++) {
		if (player_at.x >= enemy_at[i].x - 8.0f &&
			  player_at.x <= enemy_at[i].x + 8.0f &&
				player_at.y >= enemy_at[i].y - 8.0f &&
				player_at.y <= enemy_at[i].y + 8.0f) {
					hit = true;
		}
	}

	if (hit) {
		std::cout << "hit" << std::endl;
		if (difficulty > 0) {
			difficulty -= 1;
		}
	}

	// goal check
	if (player_at.x >= goal_at.x - 8.0f && 
		  player_at.x <= goal_at.x + 8.0f && 
			player_at.y >= goal_at.y - 8.0f && 
			player_at.y <= goal_at.y + 8.0f) {
		if (difficulty < MAXDIFF) {
			if (MAXDIFF - difficulty < 10) {
				difficulty = MAXDIFF;
			} else {
				difficulty += 10;
			}
		}
		goal_at.x = rand() % PPU466::ScreenWidth;
		goal_at.y = rand() % PPU466::ScreenHeight;
	}


	// background movement period
	background_period += elapsed * 3.0f;
	if (background_period >= PPU466::BackgroundWidth) {
		background_period -= PPU466::BackgroundWidth;
	}
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	// some gbc like color:
	ppu.background_color = glm::u8vec4(0x9b, 0xbc, 0x0f, 0xff);

	// draw background:
	for (uint32_t y = 0; y < PPU466::BackgroundHeight; ++y) {
		for (uint32_t x = 0; x < PPU466::BackgroundWidth; ++x) {
			ppu.background[x+PPU466::BackgroundWidth*y] = ((x + y) % 2);
		}
	}

	// background scroll:
	ppu.background_position.x = int32_t(-0.2f * player_at.x) + int32_t(background_period);
	ppu.background_position.y = int32_t(-0.2f * player_at.y) + int32_t(background_period);

  // mask sprites
	uint32_t mask_level = 1 + uint32_t(difficulty / 20.0f);
	size_t cnt = 0;
	for (uint32_t i = 0; i < mask_level; ++i) {
		for (uint32_t j = 0; j < mask_level - i; ++j) {
			ppu.sprites[10+cnt].x = int8_t(player_at.x+j);
			ppu.sprites[10+cnt+1].x = int8_t(player_at.x-j);
			ppu.sprites[10+cnt].y = int8_t(player_at.y+i);
			ppu.sprites[10+cnt+1].y = int8_t(player_at.y-i);
			ppu.sprites[10+cnt].index = 17;
			ppu.sprites[10+cnt+1].index = 17;
			ppu.sprites[10+cnt].attributes = 3;
			ppu.sprites[10+cnt+1].attributes = 3;
			ppu.sprites[10+cnt].attributes |= 0x80; //'behind' bit
			ppu.sprites[10+cnt+1].attributes |= 0x80; //'behind' bit
			cnt += 2;
		}
	}

	// player sprite:
	ppu.sprites[0].x = int8_t(player_at.x);
	ppu.sprites[0].y = int8_t(player_at.y);
	ppu.sprites[0].index = 32;
	ppu.sprites[0].attributes = 7;

	// goal sprite:
	ppu.sprites[1].x = int8_t(goal_at.x);
	ppu.sprites[1].y = int8_t(goal_at.y);
	ppu.sprites[1].index = 16;
	ppu.sprites[1].attributes = 2;

	// enemy sprites:
	for (size_t i = 0; i < MAXENEMY; ++i) {
		ppu.sprites[i+2].x = int8_t(enemy_at[i].x);
		ppu.sprites[i+2].y = int8_t(enemy_at[i].y);
		ppu.sprites[i+2].index = 33;
		ppu.sprites[i+2].attributes = 6;
		ppu.sprites[i+2].attributes |= 0x80; //'behind' bit
	}

	for (size_t i = 10 + cnt; i < 64; ++i) {
		ppu.sprites[i].y = 250;
	}

	//--- actually draw ---
	ppu.draw(drawable_size);
}
