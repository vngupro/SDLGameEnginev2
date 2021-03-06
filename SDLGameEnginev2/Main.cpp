#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 853;
const int DEAD_ZONE = 15000;
void QuitApplication(SDL_Window* window, SDL_Surface* screenSurface, SDL_Surface* currentSurface, std::vector<SDL_Surface*> images, SDL_GameController* controller);

int main(int argc, char* args[])
{
	SDL_Window* window = nullptr;
	SDL_Surface* screenSurface = nullptr;
	SDL_Surface* currentImage = nullptr;
	std::vector<SDL_Surface*> images(5, nullptr);

	//	Controller Initialization
	SDL_Joystick* joystick = nullptr;
	SDL_GameController* controller = SDL_GameControllerOpen(0);
	for (int i = 0; i < SDL_NumJoysticks(); i++)
	{
		if (SDL_IsGameController(i))
		{
			controller = SDL_GameControllerOpen(i);
			std::cout << SDL_GameControllerMapping(controller) << std::endl;
			break;
		}
	}

	// Swapping Mapping in controller
	std::cout << SDL_GameControllerAddMapping("0,X360 Controller, a:b11, b:b10") << std::endl;
	//cout << SDL_GameControllerAddMappingsFromFile("");

	// Initialize Vid?o
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		std::cout << "Video initialization error ! SDL_Error : " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	//	Collect Joystick information
	if (SDL_NumJoysticks() < 1)
	{
		std::cout << "No joystick connected " << std::endl;
	}
	else
	{
		joystick = SDL_JoystickOpen(0);
		std::cout << "Controller Name : " << SDL_JoystickName(joystick) << std::endl;
		std::cout << "Num Axes : " << SDL_JoystickNumAxes(joystick) << std::endl;
		std::cout << "Num buttons : " << SDL_JoystickNumButtons(joystick) << std::endl;
	}

	// Create Window
	// try catch instead might be good
	window = SDL_CreateWindow
	(
		"SDL Game Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	if (window == NULL)
	{
		std::cout << "Window creation error ! SLD_Error : " << SDL_GetError() << std::endl;
		QuitApplication(window, screenSurface, currentImage, images, controller);
		return 1;
	}

	// Charging Image
	screenSurface = SDL_GetWindowSurface(window);

	for (int i = 0; i < images.size(); i++)
	{
		std::string file = "IMG/LAND" + std::to_string(i + 1) + ".bmp";
		images[i] = SDL_LoadBMP(file.c_str());
		if (images[i] == NULL)
		{
			std::cout << "Image could not load ! SDL_Error : " << SDL_GetError() << std::endl;
			QuitApplication(window, screenSurface, currentImage, images, controller);
			return 1;
		}
	}
	currentImage = images[0];

	// Running Application
	bool isRunning = true;
	SDL_Event OnEvent;

	std::string text = "";
	SDL_StartTextInput();

	while (isRunning)
	{
		// Getting Quit, Mouse, Keyboard & Joystick event
		while (SDL_PollEvent(&OnEvent) != 0)
		{
			// Quit
			if (OnEvent.type == SDL_QUIT)
			{
				isRunning = false;
				std::cout << "Exit Application because you click on that cross" << std::endl;
			}
			// Text Input + Keyboard
			else if (OnEvent.type == SDL_TEXTINPUT || OnEvent.type == SDL_KEYDOWN)
			{
				system("cls");
				if (OnEvent.type == SDL_KEYDOWN && OnEvent.key.keysym.sym == SDLK_BACKSPACE && text.length() > 0)
				{
					text = text.substr(0, text.length() - 1);
				}
				else if (OnEvent.type == SDL_TEXTINPUT)
				{
					text += OnEvent.text.text;
				}
				std::cout << text << std::endl;

				switch (OnEvent.key.keysym.sym)
				{
				case SDLK_1:
					currentImage = images[0];
					break;
				case SDLK_2:
					currentImage = images[1];
					break;
				case SDLK_3:
					currentImage = images[2];
					break;
				}
			}
			// Mouse
			else if (OnEvent.type == SDL_MOUSEBUTTONDOWN)
			{
				if (OnEvent.button.button == SDL_BUTTON_LEFT)
				{
					currentImage = images[3];
					if (OnEvent.button.clicks == 2)
					{
						currentImage = images[0];
					}
					else if (OnEvent.button.state == SDL_PRESSED)
					{
						currentImage = images[3];
					}
					else if (OnEvent.button.state == SDL_RELEASED)
					{
						currentImage = images[4];
					}
				}
				else if (OnEvent.button.button == SDL_BUTTON_RIGHT)
				{
					currentImage = images[4];
				}
			}
			else if (OnEvent.type == SDL_MOUSEMOTION)
			{
				if (OnEvent.button.x < 200)
				{
					currentImage = images[1];
				}
				else if (OnEvent.button.x < 400)
				{
					currentImage = images[2];
				}
				else if (OnEvent.button.y > 100)
				{
					currentImage = images[3];
				}
				else
				{
					currentImage = images[4];
				}
			}
			// Joystick (WARNING VALUE DEPEND ON CONTROLLER)
			else if (OnEvent.type == SDL_JOYAXISMOTION)
			{
				// Checking for a specific controller not all
				if (OnEvent.jaxis.which == 0)
				{
					std::cout << OnEvent.jaxis.value << std::endl;
					// LeftAxis Horizontal
					if (OnEvent.jaxis.axis == 0)
					{
						// Left
						if (OnEvent.jaxis.value < -DEAD_ZONE)
						{
							currentImage = images[0];
						}
						// Right
						else if (OnEvent.jaxis.value > DEAD_ZONE)
						{
							currentImage = images[1];
						}
						// LeftAxis Vertical
					}
					else if (OnEvent.jaxis.axis == 1)
					{
						// Up
						if (OnEvent.jaxis.value < -DEAD_ZONE)
						{
							currentImage = images[2];
						}
						// Down
						else if (OnEvent.jaxis.value > DEAD_ZONE)
						{
							currentImage = images[3];
						}
					}
					// RigthAxis Horizontal
					else if (OnEvent.jaxis.axis == 2)
					{
						std::cout << "Trigger 2" << std::endl;
					}
					// RightAxis Vertical
					else if (OnEvent.jaxis.axis == 3)
					{
						std::cout << "Trigger 3" << std::endl;
					}
					// Left Trigger
					else if (OnEvent.jaxis.axis == 4)
					{
						// Released
						if (OnEvent.jaxis.value < -DEAD_ZONE)
						{
							currentImage = images[2];
						}
						// Pressed
						else if (OnEvent.jaxis.value > DEAD_ZONE)
						{
							currentImage = images[1];
						}
					}
					// Right Trigger
					else if (OnEvent.jaxis.axis == 5)
					{
						std::cout << "Trigger 5" << std::endl;
					}
				}
			}
			else if (OnEvent.type == SDL_JOYBUTTONDOWN)
			{
				if (OnEvent.jbutton.which == 0)
				{
					std::cout << (int)OnEvent.jbutton.button << std::endl;
					if (OnEvent.jbutton.button == 0)
					{
						std::cout << "Button A" << std::endl;
					}
				}
			}
			else if (OnEvent.type == SDL_JOYHATMOTION)
			{
				if (OnEvent.jhat.which == 0)
				{
					std::cout << (int)OnEvent.jhat.value << std::endl;
					// UP
					if (OnEvent.jhat.value == 1)
					{
						currentImage = images[1];
					}
					// RIGHT
					else if (OnEvent.jhat.value == 2)
					{
						currentImage = images[2];
					}
					// RIGHT UP
					else if (OnEvent.jhat.value == 3)
					{
						currentImage = images[3];
					}
					// DOWN
					else if (OnEvent.jhat.value == 4)
					{
						currentImage = images[4];
					}
					// RIGHT DOWN
					else if (OnEvent.jhat.value == 6)
					{
						currentImage = images[0];
					}
					// LEFT
					else if (OnEvent.jhat.value == 8)
					{
						currentImage = images[1];
					}
					// LEFT UP
					else if (OnEvent.jhat.value == 9)
					{
						currentImage = images[2];
					}
					// LEFT DOWN
					else if (OnEvent.jhat.value == 12)
					{
						currentImage = images[3];
					}
				}
			}
			else if (OnEvent.type == SDL_CONTROLLERBUTTONDOWN)
			{
				if (OnEvent.cbutton.button == SDL_CONTROLLER_BUTTON_B)
				{
					currentImage = images[2];
				}
				else if (OnEvent.cbutton.button == SDL_CONTROLLER_BUTTON_A)
				{
					currentImage = images[3];
				}
			}
		}

		SDL_BlitSurface(currentImage, NULL, screenSurface, NULL);
		SDL_UpdateWindowSurface(window);
	}

	// Quit Application
	QuitApplication(window, screenSurface, currentImage, images, controller);
	return 0;
}

void QuitApplication(SDL_Window* window, SDL_Surface* screenSurface, SDL_Surface* currentSurface, std::vector<SDL_Surface*> images, SDL_GameController* controller)
{
	if (controller != NULL)
	{
		SDL_GameControllerClose(controller);
	}
	controller = nullptr;
	screenSurface = NULL;
	for (int i = 0; i < images.size(); i++)
	{
		SDL_FreeSurface(images[i]);
		images[i] = NULL;
	}
	currentSurface = nullptr;

	SDL_StopTextInput();

	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();
}