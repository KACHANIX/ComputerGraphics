#pragma once
#include <intsafe.h>
#include <unordered_set>
#include "Keys.h"

class Game;

class InputDevice
{
private:
	Game* game = nullptr;
	std::unordered_set<Keys>* keys;

public:
	InputDevice(Game* inGame);
	~InputDevice();


	// TODO: mouse
	int MousePosition;
	int MouseOffset;
	int MouseWheelDelta = 0;
	int MouseMove;
	void OnMouseMove(int args);
	// TODO


	struct KeyboardInputEventArgs {
		/*
		 * The "make" scan code (key depression).
		 */
		USHORT MakeCode;

		/*
		 * The flags field indicates a "break" (key release) and other
		 * miscellaneous scan code information defined in ntddkbd.h.
		 */
		USHORT Flags;

		USHORT VKey;
		UINT   Message;
	};

	void OnKeyDown(KeyboardInputEventArgs args);
	void AddPressedKey(Keys key);
	void RemovePressedKey(Keys key);
	bool IsKeyDown(Keys key); 
};