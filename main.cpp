#include "libs.h"
#include "Game.h"

int main()
{
	Game game(
		"OpenGL",
		1024, 768,
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