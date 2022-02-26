#include "InputDevice.h"
#include <iostream>
#include "DisplayWin32.h"
#include "Game.h"

InputDevice::InputDevice(Game* inGame)
{
	game = inGame; 
}

InputDevice::~InputDevice()
{
	delete keys;
}

void InputDevice::OnKeyDown(KeyboardInputEventArgs args)
{
	auto key = (Keys)args.VKey;

	if (keys->count(key))
	{
		RemovePressedKey(key);
	}
	else
	{
		AddPressedKey(key);
	}

}
void InputDevice::AddPressedKey(Keys key)
{
	keys->insert(key);
}

void InputDevice::RemovePressedKey(Keys key)
{
	keys->erase(key);
}

bool InputDevice::IsKeyDown(Keys key)
{
	return keys->count(key);
}


// TODO:
void InputDevice::OnMouseMove(int args)
{
}
