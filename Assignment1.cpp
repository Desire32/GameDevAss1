#include <sstream>
#include <TL-Engine.h>	
using namespace tle;
using namespace std;

void main()
{
	float kSphereSpeed = 0.15f;
	const float kRotationSpeed = 0.05f;

	enum CameraPressed {Pressed,NotPressed};
	CameraPressed CameraState = NotPressed;

	enum GamePressed {inProcess, Win};
	GamePressed GameState = inProcess;

	enum PlayingPressed {Playing, Stop};
	PlayingPressed PlayingState = Playing;

	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	float Sphereradius = 10.0f;
	const float CubeSides = 5.0f;
	
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	IFont* myFont = myEngine->LoadFont("Comic Sans MS", 36);
	float fontX = 0;
	float fontY = 0;

	int playerPoints = 0;
	int Upgrade = 40;
	int SquareSide = 90;
	
	IMesh* waterMesh = myEngine->LoadMesh("water.x");
	IModel* water = waterMesh->CreateModel(0,-5,0);

	IMesh* islandMesh = myEngine->LoadMesh("island.x");
	IModel* island = islandMesh->CreateModel(0, -5, 0);

	IMesh* sphereMesh = myEngine->LoadMesh("spheremesh.x");
	IModel* sphere = sphereMesh->CreateModel(0, 10, 0);

	IMesh* cubeMesh = myEngine->LoadMesh("minicube.x");

	IMesh* skyboxMesh = myEngine->LoadMesh("skybox.x");
	IModel* skybox = skyboxMesh->CreateModel(0, -960, 0);

	ICamera* camera = myEngine->CreateCamera(kManual,0,200,0);
	camera->RotateLocalX(90);

	IModel* cubeArray[13];
	for (int CubeAmount = 12; CubeAmount > 0; CubeAmount--) {

		cubeArray[CubeAmount] = cubeMesh->CreateModel(rand() % 200 + (-100), rand() % 10, rand() % 200 + (-70));
		float cubeX = cubeArray[CubeAmount]->GetX() - sphere->GetX();
		float cubeY = cubeArray[CubeAmount]->GetY() - sphere->GetY();
		float cubeZ = cubeArray[CubeAmount]->GetZ() - sphere->GetZ();
		float path = sqrt(cubeX * cubeX + cubeY * cubeY + cubeZ * cubeZ);

		if (path < Sphereradius + CubeSides / 2) {
			cubeArray[CubeAmount]->SetPosition(0, -200, 0);
			playerPoints += 10;
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
		if (myEngine->KeyHeld(Key_W)) {
			sphere->MoveLocalZ(kSphereSpeed);
		}
		if (myEngine->KeyHeld(Key_S)) {
			sphere->MoveLocalZ(-kSphereSpeed);
		}
		if (myEngine->KeyHeld(Key_A)) {
			sphere->RotateLocalY(-kSphereSpeed * 2);
		}
		if (myEngine->KeyHeld(Key_D)) {
			sphere->RotateLocalY(kSphereSpeed * 2);
		}

		if (myEngine->KeyHit(Key_P) && !(GameState == Win)) {
			if (PlayingState == Playing) {
				kSphereSpeed = 0;
				PlayingState = Stop;
			}
			else {
				kSphereSpeed = 0.15f;
				PlayingState = Playing;
			}
		}

		if (myEngine->KeyHeld(Key_Y)) {
			camera->MoveZ(kSphereSpeed);
		}
		if (myEngine->KeyHeld(Key_H)) {
			camera->MoveZ(-kSphereSpeed);
		}
		if (myEngine->KeyHeld(Key_G)) {
			camera->MoveX(-kSphereSpeed);
		}
		if (myEngine->KeyHeld(Key_J)) {
			camera->MoveX(kSphereSpeed);
		}

		for (int CubeAmount = 12; CubeAmount > 0; CubeAmount--) {

			float cubeX = cubeArray[CubeAmount]->GetX() - sphere->GetX();
			float cubeY = cubeArray[CubeAmount]->GetY() - sphere->GetY();
			float cubeZ = cubeArray[CubeAmount]->GetZ() - sphere->GetZ();
			float path = sqrt(cubeX * cubeX + cubeY * cubeY + cubeZ * cubeZ);

			if (path < Sphereradius + CubeSides / 2) {
				cubeArray[CubeAmount]->SetPosition(0, -200, 0);
				playerPoints += 10;
			}
		}

		stringstream outText;
		if (GameState == inProcess) {
		outText << "Points:" << playerPoints;
		myFont->Draw(outText.str(), fontX, fontY, kBlack);
		outText.str("");

		if (playerPoints >= 50){
			outText << "You Won!";
			myFont->Draw(outText.str(), 560.0f, 335.0f, kRed);
			kSphereSpeed = 0;
		}

		if (!(sphere->GetX() < SquareSide && sphere->GetX() > -SquareSide) || !(sphere->GetZ() < SquareSide && sphere->GetZ() > -SquareSide)) {
			outText << "You LOSE!";
			myFont->Draw(outText.str(), 560.0f, 335.0f, kRed);
			kSphereSpeed = 0;
		}
		}

		if (playerPoints >= Upgrade) {
			sphere->Scale(1.2f);
			Upgrade += Upgrade;
			Sphereradius *= 1.2;
			sphere->MoveY(5);
		}


		if (myEngine->KeyHit(Key_2)) {
			if (CameraState == NotPressed) {
				camera->ResetOrientation();
				camera->SetPosition(150, 150, -150);
				camera->RotateLocalY(-45);
				camera->RotateLocalX(45);
				CameraState = Pressed;
			}
		}
		if (myEngine->KeyHit(Key_1)) {
			if (CameraState == Pressed) {
				camera->ResetOrientation();
				camera->SetPosition(0, 200, 0);
					camera->RotateLocalX(90);
					CameraState = NotPressed;
			}
		}
	}
	myEngine->Delete();
}
