#include "PPU466.hpp"
#include "Mode.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

// limits
const uint8_t MAXENEMY = 8;
const uint16_t MAXDIFF = 80;

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	//player position:
	glm::vec2 player_at = glm::vec2(PPU466::ScreenWidth/2.0f, PPU466::ScreenHeight/2.0f);

	//goal position:
	glm::vec2 goal_at = glm::vec2(PPU466::ScreenWidth/2.0f, PPU466::ScreenHeight/4.0f);

	//enemy position:

	std::array< glm::vec2, MAXENEMY > enemy_at = {glm::vec2(PPU466::ScreenWidth/4.0f,      PPU466::ScreenHeight/4.0f),
																         glm::vec2(PPU466::ScreenWidth/4.0f,      PPU466::ScreenHeight/2.0f),
																         glm::vec2(PPU466::ScreenWidth/2.0f,      PPU466::ScreenHeight/4.0f),
																         glm::vec2(PPU466::ScreenWidth*3.0f/4.0f, PPU466::ScreenHeight/4.0f),
																         glm::vec2(PPU466::ScreenWidth/4.0f,      PPU466::ScreenHeight*3.0f/4.0f),
																         glm::vec2(PPU466::ScreenWidth*3.0f/4.0f, PPU466::ScreenHeight/2.0f),
																         glm::vec2(PPU466::ScreenWidth/2.0f,      PPU466::ScreenHeight*3.0f/2.0f),
																         glm::vec2(PPU466::ScreenWidth*3.0f/4.0f, PPU466::ScreenHeight*3.0f/4.0f)};

	std::array< glm::vec2, MAXENEMY > enemy_direc;

	//background moving period:
	float background_period = 0.0f;

	//difficulty: decides the scale of the mask
	uint16_t difficulty = 0;

	//----- drawing handled by PPU466 -----

	PPU466 ppu;
};
