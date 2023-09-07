// Author: Sirui Huang
// The Blocks asset

// Block is the main asset for Seek, where all the enemies, the player, 
// the walls, and the goal are all represented by blocks.
 
#pragma once

#include <glm/glm.hpp>
#include "load_save_png.hpp"

#include <vector>
#include <string>
#include <cassert>
#include <stdint.h>
#include <stdio.h>

// Notes:
// - Blocks will be made as png images
// - There are three types of Movable Blocks: player, enemy, and goal (for now)
// - Some blocks might be represented by multiple sprites.
  
// Restrict the type of the block
enum block_type : uint8_t {
  PLAYER_BLOCK,
  ENEMY_BLOCK,
  GOAL_BLOCK
};

struct Block {
  // constructor
  Block (std::string const &path) {
    // load the png file
    glm::uvec2 png_size;
    std::vector< glm::u8vec4 > png_data;
    load_png(path, &png_size, &png_data, LowerLeftOrigin);

    // fill in patterns according to alpha values
    sprites.clear();
    assert(png_size.x == 8 && png_size.y == 8 && "The size of the png file is not 8x8");
    uint8_t i = 0;
    while (i < png_size.x) {
      uint8_t temp = 0b0;
      uint8_t j = 0;
      while (j < png_size.y) {
        temp <<= 1;
        if (png_data[i * png_size.y + j].a >= 128) {
          temp += 1;
        } 
        j += 1;
      }
      sprites.push_back(temp);
      i += 1;
    }
    assert(sprites.size() == 8 && "The number of sprites is not 8");

    // set the type of the block
    if (path.find("eye") != std::string::npos) {
      type = PLAYER_BLOCK;
    } else if (path.find("enemy") != std::string::npos) {
      type = ENEMY_BLOCK;
    } else if (path.find("goal") != std::string::npos) {
      type = GOAL_BLOCK;
    } else {
      assert(false && "Undefined block type");
    }

    // set the palette
    palette = new std::array< glm::u8vec4, 4 >;
    (*palette)[0] = glm::u8vec4(0x00, 0x00, 0x00, 0x00);
    (*palette)[1] = glm::u8vec4(0x00, 0x00, 0x00, 0xff);
    (*palette)[2] = glm::u8vec4(0x00, 0x00, 0x00, 0xff);
    (*palette)[3] = glm::u8vec4(0x00, 0x00, 0x00, 0xff);
  }

  // A block might be represented by multiple sprites (e.g. the eyeball!)
  std::vector< uint8_t > sprites;

  // Color Pallete of the constituent tiles
  std::array< glm::u8vec4, 4 > *palette;

  // Get the type of the block
  block_type type;
};