#include <TL-Engine.h>	
#include <sstream>
#include <cmath>
using namespace tle;
using namespace std;

void main()
{
	enum GamePressed {inProcess, Finished};
	GamePressed GameState = inProcess;

	enum CubeTaken {Hyper, Regular};
	CubeTaken SphereState = Regular;

	enum PausePressed { Playing, Paused };
	PausePressed PauseState = Playing;

	enum CameraPressed {Normal, Isometric};
	CameraPressed CameraState = Normal;

	enum enemySphere {Gorizontal, Vertical};
	enemySphere enemySphereState = Gorizontal;

	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");
	IFont* myFont = myEngine->LoadFont("Comic Sans MS", 36);

	float kGameSpeed = 70.0f;
	float Sphereradius = 10.0f;
	float enemySphereTimer = 0;
	const float CubeSides = 5.0f;
	float sphereTimer = 0;
	const float HypermodeLifeTime = 5.0f;
	const float kPi = 3.1415926f;
	const float enemySphereSpeed = 0.65f;
	float kRadius = 50.0f;
	float enemySphereDegrees = 0.0f;
	
	int playerPoints = 0;
	int enemyPoints = 0;
	int Upgrade = 40;
	int enemyUpgrade = 40;
	const int SquareSide = 90;
	const int CubeCollision = 10;
	int CubeAmount = 11;
	
	IMesh* waterMesh = myEngine->LoadMesh("water.x");
	IMesh* islandMesh = myEngine->LoadMesh("island.x");
	IMesh* sphereMesh = myEngine->LoadMesh("spheremesh.x");
	IMesh* cubeMesh = myEngine->LoadMesh("minicube.x");
	IMesh* skyboxMesh = myEngine->LoadMesh("skybox.x");
	IMesh* enemySphereMesh = myEngine->LoadMesh("sphere.x");
	
	IModel* cubeArray[12];
	IModel* water = waterMesh->CreateModel(0, -5, 0);
	IModel* island = islandMesh->CreateModel(0, -5, 0);
	IModel* skybox = skyboxMesh->CreateModel(0, -960, 0);
	IModel* sphere = sphereMesh->CreateModel(0, 10, -35);
	IModel* hypercube = cubeMesh->CreateModel(rand() % 200 + (-90), 5, rand() % 220 + (-120));
	hypercube->SetSkin("hypercube.jpg");
	IModel* enemySphere = enemySphereMesh->CreateModel(0, 10, 15);
	enemySphere->SetSkin("enemysphere.jpg");

	ICamera* camera = myEngine->CreateCamera(kManual,0,220,0);
	camera->RotateLocalX(90); 

	for (int CubeAmount = 11; CubeAmount >= 0; CubeAmount--) {
		cubeArray[CubeAmount] = cubeMesh->CreateModel(rand() % 200 + (-90), 5, rand() % 220 + (-120));
	}
	for (int CubeAmount = 11; CubeAmount > 0; CubeAmount--) {
		float cubePathX = cubeArray[CubeAmount]->GetX() - cubeArray[CubeAmount - 1]->GetX();
		float cubePathZ = cubeArray[CubeAmount]->GetZ() - cubeArray[CubeAmount - 1]->GetZ();
		float cubePath = sqrt(cubePathX * cubePathX + cubePathZ * cubePathZ);

		while (cubePath < CubeCollision) {
			cubeArray[CubeAmount]->SetPosition(rand() % 200 + (-90), 5, rand() % 220 + (-120));
			cubePathX = cubeArray[CubeAmount]->GetX() - cubeArray[CubeAmount - 1]->GetX();
			cubePathZ = cubeArray[CubeAmount]->GetZ() - cubeArray[CubeAmount - 1]->GetZ();
			cubePath = sqrt(cubePathX * cubePathX + cubePathZ * cubePathZ);
		}
	}

	myEngine->Timer();
	while (myEngine->IsRunning())
	{
		myEngine->DrawScene();
		float deltaTime = myEngine->Timer();

		if (myEngine->KeyHit(Key_Escape)) {
			break;
		}

		if (enemySphereState == Gorizontal) {
			enemySphereTimer += deltaTime;

			enemySphereDegrees += 0.1f * enemySphereSpeed;
			if (enemySphereDegrees > 360.0f) {
				enemySphereDegrees = 0.0f;
			}
			float radians = kPi / 180 * enemySphereDegrees;

			float horizontalRadius = kRadius * 1.5;
			float verticalRadius = kRadius;

			float xPos = horizontalRadius * cos(radians);
			float yPos = verticalRadius * sin(2 * radians);

			enemySphere->SetPosition(xPos, 0, yPos);
			if (enemySphereTimer > 3) {
				enemySphereState = Vertical;
				enemySphereTimer = 0;
			}
		}
		if (enemySphereState == Vertical) {
			enemySphereTimer += deltaTime;
			
			enemySphereDegrees += 0.1f * enemySphereSpeed;
			if (enemySphereDegrees > 360.0f) {
				enemySphereDegrees = 0.0f;
			}
			float radians = kPi / 180 * enemySphereDegrees;

			float horizontalRadius = kRadius * 1.5;
			float verticalRadius = kRadius;

			float xPos = horizontalRadius * cos(radians);
			float yPos = verticalRadius * sin(2 * radians);

			enemySphere->SetPosition(yPos, 0, xPos);
			if (enemySphereTimer > 3) {
				enemySphereState = Gorizontal;
			    enemySphereTimer = 0;
			}
		}
		
		stringstream outText, enemyText;
		if (GameState == inProcess) {

			if (myEngine->KeyHeld(Key_W)) {
				sphere->MoveLocalZ(kGameSpeed * deltaTime);
			}
			if (myEngine->KeyHeld(Key_S)) {
				sphere->MoveLocalZ(-kGameSpeed * deltaTime);
			}
			if (myEngine->KeyHeld(Key_A)) {
				sphere->RotateLocalY(-kGameSpeed * deltaTime * 1.5);
			}
			if (myEngine->KeyHeld(Key_D)) {
				sphere->RotateLocalY(kGameSpeed * deltaTime * 1.5);
			}

			if (CameraState == Normal) {
				if (myEngine->KeyHeld(Key_Up)) {
					camera->MoveZ(kGameSpeed * deltaTime);
				}
				else if (myEngine->KeyHeld(Key_Down)) {
					camera->MoveZ(kGameSpeed * -deltaTime);
				}
				else if (myEngine->KeyHeld(Key_Left)) {
					camera->MoveX(-deltaTime * kGameSpeed);
				}
				else if (myEngine->KeyHeld(Key_Right)) {
					camera->MoveX(deltaTime * kGameSpeed);
				}
			}

			if (myEngine->KeyHit(Key_P)) { 
				if (PauseState == Playing) {
					PauseState = Paused;
					kGameSpeed = 0;
				}
				else {
					kGameSpeed = 70.0f;
					PauseState = Playing;

				}
			}

			if (PauseState != Paused) {
				if (myEngine->KeyHit(Key_2)) {
					camera->ResetOrientation();
					camera->SetPosition(150, 150, -150);
					camera->RotateLocalY(-45);
					camera->RotateLocalX(45);
					CameraState = Isometric;
				}
				if (myEngine->KeyHit(Key_1)) {
					camera->ResetOrientation();
					camera->SetPosition(0, 220, 0);
					camera->RotateLocalX(90);
					CameraState = Normal;
				}
			}

			int posPlayer = (playerPoints > enemyPoints) ? 10 : 60;
			int posEnemy = (playerPoints > enemyPoints) ? 60 : 10;

			outText << "Player points: " << playerPoints;
			myFont->Draw(outText.str(), 1025, posPlayer, kBlue);
			outText.str("");

			enemyText << "Enemy points: " << enemyPoints;
			myFont->Draw(enemyText.str(), 1025, posEnemy, kWhite);
			enemyText.str("");

			if (SphereState == Regular) {
				float hyperX = hypercube->GetX() - sphere->GetX();
				float hyperZ = hypercube->GetZ() - sphere->GetZ();
				float hyperPath = sqrt(hyperX * hyperX + hyperZ * hyperZ);
				if (hyperPath < Sphereradius + CubeSides / 2) {
					hypercube->SetPosition(-1000, -200, 0);
					sphere->SetSkin("hypersphere.jpg");
					SphereState = Hyper;
				}
			}
			if (SphereState == Hyper) {
				sphereTimer += deltaTime;
				if (sphereTimer > HypermodeLifeTime) {
					sphere->SetSkin("regularsphere.jpg");
					sphereTimer = 0;
					SphereState = Regular;
				}
			}
			for (int CubeAmount = 11; CubeAmount >= 0; CubeAmount--) { 
				float cubeX = cubeArray[CubeAmount]->GetX() - sphere->GetX();
				float cubeZ = cubeArray[CubeAmount]->GetZ() - sphere->GetZ();
				float path = sqrt(cubeX * cubeX + cubeZ * cubeZ);

				float sphereCollisionX = sphere->GetX() - enemySphere->GetX();
				float sphereCollisionZ = sphere->GetZ() - enemySphere->GetZ();
				float collisionPath = sqrt(sphereCollisionX * sphereCollisionX + sphereCollisionZ * sphereCollisionZ);

				if (collisionPath < Sphereradius + CubeSides / 2) {
					if (playerPoints - enemyPoints >= 40) {
						enemySphere->SetPosition(-1000, -1000, 0);
						playerPoints += 40;
					}
					if (enemyPoints - playerPoints >= 40) {
						enemyPoints += 40;
						sphere->SetPosition(-1000, -1000, 0);
					}
					else {
						enemySphere->Move(sphereCollisionX * -deltaTime * 3, 0, sphereCollisionZ * deltaTime);
						sphere->Move(sphereCollisionX * -deltaTime, 0, sphereCollisionZ * deltaTime * 3);
					}
				}
				if (path < Sphereradius + CubeSides / 2) {
					cubeArray[CubeAmount]->SetPosition(rand() % 200 + (-90), 5, rand() % 220 + (-110));
					playerPoints += 10;
				}
				if (path < 50 && SphereState == Hyper) {
					cubeArray[CubeAmount]->Move(cubeX * -deltaTime, 0, cubeZ * -deltaTime);
				}
			}
		}

		for (int CubeAmount = 11; CubeAmount >= 0; CubeAmount--) {
			float enemyCubeX = enemySphere->GetX() - cubeArray[CubeAmount]->GetX();
			float enemyCubeZ = enemySphere->GetZ() - cubeArray[CubeAmount]->GetZ();
			float enemyPath = sqrt(enemyCubeX * enemyCubeX + enemyCubeZ * enemyCubeZ);

			if (enemyPath < Sphereradius + CubeSides / 2) {
				cubeArray[CubeAmount]->SetPosition(rand() % 200 + (-90), 5, rand() % 220 + (-110));
				enemyPoints += 10;
			}
		}

		if (playerPoints >= Upgrade) {
			sphere->Scale(1.2f);
			Upgrade += Upgrade;
			Sphereradius *= 1.2;
			sphere->MoveY(5);
		}
		if (playerPoints >= 120 || !(enemySphere->GetX() < SquareSide && enemySphere->GetX() > -SquareSide) || 
			!(enemySphere->GetZ() < SquareSide && enemySphere->GetZ() > -SquareSide)) {
			outText << "You Won!";
			myFont->Draw(outText.str(), 590.0f, 335.0f, kRed);
			GameState = Finished;
		}
		if (enemyPoints >= enemyUpgrade) {
			enemySphere->Scale(1.2f);
			enemyUpgrade += enemyUpgrade;
			Sphereradius *= 1.2;
			enemySphere->MoveY(5);
		}
		if (!(sphere->GetX() < SquareSide && sphere->GetX() > -SquareSide) ||
			!(sphere->GetZ() < SquareSide && sphere->GetZ() > -SquareSide) || enemyPoints >= 120)
		{
			outText << "Game over";
			myFont->Draw(outText.str(), 590.0f, 335.0f, kRed);
			GameState = Finished;
		}
	}
	myEngine->Delete();
}
