#include <TL-Engine.h>	
#include <sstream>
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
	
	int playerPoints = 0;
	int enemyPoints = 0;
	int Upgrade = 40;
	int enemyUpgrade = 40;
	const int SquareSide = 90;
	const int CubeCollision = 10;
	int CubeAmount = 11;
	
	IMesh* waterMesh = myEngine->LoadMesh("water.x");
	IModel* water = waterMesh->CreateModel(0,-5,0);

	IMesh* islandMesh = myEngine->LoadMesh("island.x");
	IModel* island = islandMesh->CreateModel(0, -5, 0);

	IMesh* sphereMesh = myEngine->LoadMesh("spheremesh.x");
	IModel* sphere = sphereMesh->CreateModel(0, 10, 0);
	
	IMesh* cubeMesh = myEngine->LoadMesh("minicube.x");

	IModel* hypercube = cubeMesh->CreateModel(rand() % 200 + (-100), 5, rand() % 200 + (-100));
	hypercube->SetSkin("hypercube.jpg");
	
	IMesh* skyboxMesh = myEngine->LoadMesh("skybox.x");
	IModel* skybox = skyboxMesh->CreateModel(0, -960, 0);

	IMesh* enemySphereMesh = myEngine->LoadMesh("sphere.x");
	IModel* enemySphere = enemySphereMesh->CreateModel(0,10,0);
	enemySphere->SetSkin("enemysphere.jpg");

	ICamera* camera = myEngine->CreateCamera(kManual,0,200,0);
	camera->RotateLocalX(90);
	IModel* cubeArray[12]; 

	for (int CubeAmount = 11; CubeAmount >= 0; CubeAmount--) {
		cubeArray[CubeAmount] = cubeMesh->CreateModel(rand() % 200 + (-100), 5, rand() % 220 + (-115));
	}

	myEngine->Timer();
	while (myEngine->IsRunning())
	{
		myEngine->DrawScene();
		float deltaTime = myEngine->Timer();

		if (myEngine->KeyHit(Key_Escape)) {
			break;
		}

		for (int CubeAmount = 11; CubeAmount > 0; CubeAmount--) {
			float cubePathX = cubeArray[CubeAmount]->GetX() - cubeArray[CubeAmount - 1]->GetX();
			float cubePathZ = cubeArray[CubeAmount]->GetZ() - cubeArray[CubeAmount - 1]->GetZ();
			float cubePath = sqrt(cubePathX * cubePathX + cubePathZ * cubePathZ);

			while (cubePath < CubeCollision) {
				cubeArray[CubeAmount]->SetPosition(rand() % 200 + (-100), 5, rand() % 200 + (-100));
				cubePathX = cubeArray[CubeAmount]->GetX() - cubeArray[CubeAmount - 1]->GetX();
				cubePathZ = cubeArray[CubeAmount]->GetZ() - cubeArray[CubeAmount - 1]->GetZ();
				cubePath = sqrt(cubePathX * cubePathX + cubePathZ * cubePathZ);
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
					camera->SetPosition(0, 200, 0);
					camera->RotateLocalX(90);
					CameraState = Normal;
				}
			}

			outText << "Points: " << playerPoints;
			myFont->Draw(outText.str(), 20, 50, kBlue);
			outText.str("");

			enemyText << "Enemy points: " << enemyPoints;
			myFont->Draw(enemyText.str(), 1025 , 50, kWhite);
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
			
			for (int CubeAmount = 11; CubeAmount >= 0; CubeAmount--) { // FIX

				float cubeX = cubeArray[CubeAmount]->GetX() - sphere->GetX();
				float cubeZ = cubeArray[CubeAmount]->GetZ() - sphere->GetZ();
				float path = sqrt(cubeX * cubeX + cubeZ * cubeZ);

				float enemyCubeX = enemySphere->GetX() - cubeArray[CubeAmount]->GetX();
				float enemyCubeZ = enemySphere->GetZ() - cubeArray[CubeAmount]->GetZ();
				float enemyPath = sqrt(enemyCubeX * enemyCubeX + enemyCubeZ * enemyCubeZ);

				float sphereCollisionX = sphere->GetX() - enemySphere->GetX();
				float sphereCollisionZ = sphere->GetZ() - enemySphere->GetZ();
				float collisionPath = sqrt(sphereCollisionX * sphereCollisionX + sphereCollisionZ * sphereCollisionZ);

				if (enemyPath < 50 && !(PauseState == Paused)) {
					enemySphere->Move(enemyCubeX * deltaTime, 0, -enemyCubeZ * deltaTime);
				}

				if (collisionPath < Sphereradius + CubeSides / 2) {
					if (playerPoints - enemyPoints >= 40) {
						enemySphere->SetPosition(-1000, -1000, 0);
						playerPoints += 40;
					}
					else { // FIX, 60%;
						//enemySphere->Move(sphereCollisionX * -deltaTime, 0, sphereCollisionZ * deltaTime);
						//sphere->Move(sphereCollisionX * -deltaTime, 0, sphereCollisionZ * deltaTime);
						enemySphere->MoveZ(sphereCollisionZ * deltaTime);
						sphere->MoveZ(sphereCollisionZ * deltaTime * 2);
					}
				}
				if (enemyPath < Sphereradius + CubeSides / 2) {
					cubeArray[CubeAmount]->SetPosition(rand() % 200 + (-100), 5, rand() % 200 + (-100));
					enemyPoints += 10;
				}
				if (path < Sphereradius + CubeSides / 2) {
					cubeArray[CubeAmount]->SetPosition(rand() % 200 + (-100), 5, rand() % 200 + (-100));
					playerPoints += 10;
				}
				if (path < 50 && SphereState == Hyper) {
					cubeArray[CubeAmount]->Move(cubeX * -deltaTime, 0, cubeZ * -deltaTime);
				}
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
			myFont->Draw(outText.str(), 560.0f, 335.0f, kRed);
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
			myFont->Draw(outText.str(), 560.0f, 335.0f, kRed);
			GameState = Finished;
		}
	}
	myEngine->Delete();
}
