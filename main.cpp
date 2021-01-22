#include "libs.h"
#include "Game.h"

int main()
{
	Game game(
		"OpenGL",
		640, 480,
		4, 4,
		false);

	// MAIN LOOP
	while (!game.getWindowShouldClose())
	{
		// UPDATE INPUT ---
		game.update();
		game.render();
	}

	return 0;
}