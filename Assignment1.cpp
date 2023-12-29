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

	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");

	float kGameSpeed = 70.0f;
	float Sphereradius = 10.0f;
	float enemySphereTimer = 0;
	const float CubeSides = 5.0f;
	float sphereTimer = 0;
	const float HypermodeLifeTime = 5.0f;

	IFont* myFont = myEngine->LoadFont("Comic Sans MS", 36);
	
	int playerPoints = 0;
	int enemyPoints = 0;
	int Upgrade = 40;
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
	IModel* enemySphere = enemySphereMesh->CreateModel();
	enemySphere->SetSkin("enemysphere.jpg");

	ICamera* camera = myEngine->CreateCamera(kManual,0,200,0);
	camera->RotateLocalX(90);
	IModel* cubeArray[12]; //kinda working
	for (int CubeAmount = 11; CubeAmount >= 0; CubeAmount--) {
		cubeArray[CubeAmount] = cubeMesh->CreateModel(rand() % 200 + (-100), 5, rand() % 200 + (-100));
	}
	
	do {
		float cubePathX = cubeArray[CubeAmount]->GetX() - cubeArray[CubeAmount - 1]->GetX();
		float cubePathZ = cubeArray[CubeAmount]->GetZ() - cubeArray[CubeAmount - 1]->GetZ();
		float cubePath = sqrt(cubePathX * cubePathX + cubePathZ * cubePathZ);
		if (cubePath < CubeCollision) {
			cubeArray[CubeAmount]->SetPosition(rand() % 200 + (-100), 5, rand() % 200 + (-100));
		}
		CubeAmount--;
		} while (CubeAmount > 0);
	myEngine->Timer();
	while (myEngine->IsRunning())
	{
		myEngine->DrawScene();
		float deltaTime = myEngine->Timer();

		if (myEngine->KeyHit(Key_Escape)) {
			break;
		}
	
		stringstream outText, enemyText, pause;
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

		if (myEngine->KeyHit(Key_P)) { // FIX LATER, UNNECESSARY
			if (PauseState == Playing) {
				outText << "Pause";
				myFont->Draw(outText.str(), 560.0f, 335.0f, kBlue);
				outText.str("");
				PauseState = Paused;
				kGameSpeed = 0;
			}
			if (PauseState == Paused) {
				kGameSpeed = 70.0f;
			}
		}

		outText << "Points:" << playerPoints;
		myFont->Draw(outText.str(), 0, 0, kBlack);
		outText.str("");

		enemyText << "Enemy points: " << enemyPoints;
		myFont->Draw(enemyText.str(), 0, 100, kBlack);
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
		if (myEngine->KeyHit(Key_2)) {
				camera->ResetOrientation();
				camera->SetPosition(150, 150, -150);
				camera->RotateLocalY(-45);
				camera->RotateLocalX(45);
		}
		if (myEngine->KeyHit(Key_1)) {
				camera->ResetOrientation();
				camera->SetPosition(0, 200, 0);
				camera->RotateLocalX(90);
		}
		for (int CubeAmount = 11; CubeAmount >= 0; CubeAmount--) { // FIX
			float cubeX = cubeArray[CubeAmount]->GetX() - sphere->GetX();
			float cubeZ = cubeArray[CubeAmount]->GetZ() - sphere->GetZ();
			float path = sqrt(cubeX * cubeX + cubeZ * cubeZ);

			float enemyCubeX = enemySphere->GetX() - cubeArray[CubeAmount]->GetX();
			float enemyCubeZ = enemySphere->GetZ() - cubeArray[CubeAmount]->GetZ();
			float enemyPath = sqrt(enemyCubeX * enemyCubeX + enemyCubeZ * enemyCubeZ);

			if (enemyPath < 50) {
				enemySphere->LookAt(cubeArray[CubeAmount]);
				enemySphere->Move(enemyCubeX * deltaTime, 0, -enemyCubeZ * deltaTime);
			}

			/*float dotProduct = (enemySphere->GetX() * cubeArray[CubeAmount]->GetX() + enemySphere->GetZ() * cubeArray[CubeAmount]->GetZ());
			float crossProductX = enemySphere->GetY() * cubeArray[CubeAmount]->GetZ() - enemySphere->GetZ() * cubeArray[CubeAmount]->GetY();
			float crossProductY = enemySphere->GetZ() * cubeArray[CubeAmount]->GetX() - enemySphere->GetX() * cubeArray[CubeAmount]->GetZ();
			float crossProductZ = enemySphere->GetX() * cubeArray[CubeAmount]->GetY() - enemySphere->GetY() * cubeArray[CubeAmount]->GetX();
			
				enemySphere->Move(crossProductX * deltaTime, 0, crossProductZ * deltaTime);
			
			enemySphereTimer += deltaTime;
			enemySphere->Move(rand() % 200 + (-100), 0, rand() % 200 + (-100));
			if (enemySphereTimer > 5) {
				enemySphereTimer = 0;
				enemySphere->Move(rand() % 200 + (-100), 0, rand() % 200 + (-100));
			}
			*/

			
			
			
			


			
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

		if (playerPoints >= 1020) {
			outText << "You Won!";
			myFont->Draw(outText.str(), 560.0f, 335.0f, kRed);
			GameState = Finished;
		}
		if (!(sphere->GetX() < SquareSide && sphere->GetX() > -SquareSide) || !(sphere->GetZ() < SquareSide && sphere->GetZ() > -SquareSide)) {
			outText << "Game over";
			myFont->Draw(outText.str(), 560.0f, 335.0f, kRed);
			GameState = Finished;
		}
		if (playerPoints >= Upgrade) {
			sphere->Scale(1.2f);
			Upgrade += Upgrade;
			Sphereradius *= 1.2;
			sphere->MoveY(5);
		}
	}
	myEngine->Delete();
}
