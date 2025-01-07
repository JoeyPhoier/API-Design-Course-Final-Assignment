#pragma once
#include "MyTexture2D.h"
#include "MyAnimation2D.h"

struct TextureLibrary
{
	MyAnimation2D playerAnimation = MyAnimation2D("./Assets/PlayerShip.png", "./Assets/PlayerShip.txt");
	MyTexture2D alienTexture = MyTexture2D("./Assets/Alien.png");
	MyTexture2D projectileTexture = MyTexture2D("./Assets/Laser.png");
	MyTexture2D barrierTexture = MyTexture2D("./Assets/Barrier.png");
	MyTexture2D backgroundTexture = MyTexture2D("./Assets/Space Background.png");
};