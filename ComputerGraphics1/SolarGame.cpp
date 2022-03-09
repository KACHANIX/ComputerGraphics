#include "SolarGame.h"

#include "BoxComponent.h"
#include "Camera.h"
#include "FPSCameraController.h"
#include "PlaneComponent.h"
#include "SphereComponent.h"

BoxComponent* box_1;

BoxComponent* box_2;
BoxComponent* box_3;

SphereComponent* sphere_1;
SphereComponent* sphere_2;
SphereComponent* sphere_3;
SphereComponent* sphere_4;
SphereComponent* sphere_5;

BoxComponent* box_4;
BoxComponent* box_5;
SphereComponent* sphere_6;

PlaneComponent* plane;
void SolarGame::Initialize()
{
	cam = new Camera(this);
	cam_controller = new FPSCameraController(this, cam);
	plane = new PlaneComponent(this, cam, 50);
	box_1 = new BoxComponent(this, cam);
	box_2 = new BoxComponent(this, cam, box_1, 20);
	box_3 = new BoxComponent(this, cam, box_2, 6); 

	sphere_1 = new SphereComponent(this, cam, box_1, 42);
	sphere_2 = new SphereComponent(this, cam, sphere_1, 10);
	sphere_3 = new SphereComponent(this, cam, sphere_1, -10);
	sphere_4 = new SphereComponent(this, cam, sphere_1, 6);
	sphere_5 = new SphereComponent(this, cam, sphere_1, -6);
	 
	sphere_6 = new SphereComponent(this, cam, box_1, 70); 
	box_4 = new BoxComponent(this, cam, sphere_6, 8);
	box_5 =  new BoxComponent(this, cam, sphere_6, -8);
	components.push_back(plane);
	components.push_back(box_1);
	components.push_back(box_2);
	components.push_back(box_3);
	components.push_back(sphere_1);
	components.push_back(sphere_2);
	components.push_back(sphere_3);
	components.push_back(sphere_4);
	components.push_back(sphere_5);
	components.push_back(sphere_6);
	components.push_back(box_4);
	components.push_back(box_5); 
}

void SolarGame::Update(float delta_time)
{
	cam_controller->Update(delta_time);
	Game::Update(delta_time);
}

