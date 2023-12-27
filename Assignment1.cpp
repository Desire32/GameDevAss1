#include <sstream>
#include <TL-Engine.h>	
using namespace tle;
using namespace std;

/*struct Vectors {
	float floatX = 0;
	float floatY = 0;
	float floatZ = 0;
};

void collectPositions(IModel* model, Vectors ModelPositions) {

	ModelPositions.floatX = model->GetX();
	ModelPositions.floatY = model->GetY();
	ModelPositions.floatZ = model->GetZ();
}

float dotProductCalculate(Vectors v, Vectors w) {
	return v.floatX * w.floatX + v.floatY * w.floatY + v.floatZ * w.floatZ;
}*/

void main()
{
	float kSphereSpeed = 70.0f;

	enum GamePressed {inProcess, Finished};
	GamePressed GameState = inProcess;

	enum CubeTaken {Hyper, Regular};
	CubeTaken SphereState = Regular;

	enum PlayingPressed { Playing, Paused };
	PlayingPressed PlayingState = Playing;

	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	float Sphereradius = 10.0f;
	const float CubeSides = 5.0f;
	float sphereTimer = 0;
	const float HypermodeLifeTime = 5.0f;
	
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	IFont* myFont = myEngine->LoadFont("Comic Sans MS", 36);
	
	int playerPoints = 0;
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
	
	IMesh* skyboxMesh = myEngine->LoadMesh("skybox.x");
	IModel* skybox = skyboxMesh->CreateModel(0, -960, 0);

	ICamera* camera = myEngine->CreateCamera(kManual,0,200,0);
	camera->RotateLocalX(90);

	IModel* cubeArray[12]; //kinda working
	for (int CubeAmount = 11; CubeAmount >= 0; CubeAmount--) {
		cubeArray[CubeAmount] = cubeMesh->CreateModel();
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

	IModel* hypercube = cubeMesh->CreateModel(rand() % 200 + (-100), 5, rand() % 200 + (-100));
	hypercube->SetSkin("hypercube.jpg");

	myEngine->Timer();
	while (myEngine->IsRunning())
	{
		myEngine->DrawScene();
		float deltaTime = myEngine->Timer();

		if (myEngine->KeyHit(Key_Escape)) {
			break;
		}
		if (myEngine->KeyHeld(Key_W)) {
			sphere->MoveLocalZ(kSphereSpeed * deltaTime);
		}
		if (myEngine->KeyHeld(Key_S)) {
			sphere->MoveLocalZ(-kSphereSpeed * deltaTime);
		}
		if (myEngine->KeyHeld(Key_A)) {
			sphere->RotateLocalY(-kSphereSpeed * deltaTime * 1.5);
		}
		if (myEngine->KeyHeld(Key_D)) {
			sphere->RotateLocalY(kSphereSpeed * deltaTime * 1.5);
		}
		if (myEngine->KeyHeld(Key_Up)) {
			camera->MoveZ(myEngine->GetWidth() / 5 * deltaTime);
		}
		else if (myEngine->KeyHeld(Key_Down)) {
			camera->MoveZ(myEngine->GetWidth() / 5 * -deltaTime);
		}
		else if (myEngine->KeyHeld(Key_Left)) {
			camera->MoveX(-deltaTime * myEngine->GetWidth() / 5);
		}
		else if (myEngine->KeyHeld(Key_Right)) {
			camera->MoveX(deltaTime * myEngine->GetWidth() / 5);
		}

		stringstream outText;
		if (GameState == inProcess) {
		outText << "Points:" << playerPoints;
		myFont->Draw(outText.str(), 0, 0, kBlack);
		outText.str("");

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

		for (int CubeAmount = 11; CubeAmount >= 0; CubeAmount--) {
			float cubeX = cubeArray[CubeAmount]->GetX() - sphere->GetX();
			float cubeZ = cubeArray[CubeAmount]->GetZ() - sphere->GetZ();
			float path = sqrt(cubeX * cubeX + cubeZ * cubeZ);

			if (path < Sphereradius + CubeSides / 2) {
				cubeArray[CubeAmount]->SetPosition(rand() % 200 + (-100), 5, rand() % 200 + (-100));
				playerPoints += 10;
			}
			if (path < 50 && SphereState == Hyper) { 
				// cubeArray[CubeAmount]->LookAt(sphere);
				cubeArray[CubeAmount]->Move(cubeX * -deltaTime, 0, cubeZ * -deltaTime);
			}
		}
		}

		if (playerPoints >= 120) {
			outText << "You Won!";
			myFont->Draw(outText.str(), 560.0f, 335.0f, kRed);
			kSphereSpeed = 0;
			GameState = Finished;
		}
		if (!(sphere->GetX() < SquareSide && sphere->GetX() > -SquareSide) || !(sphere->GetZ() < SquareSide && sphere->GetZ() > -SquareSide)) {
			outText << "You LOSE!";
			myFont->Draw(outText.str(), 560.0f, 335.0f, kRed);
			kSphereSpeed = 0;
			GameState = Finished;
		}
		if (playerPoints >= Upgrade) {
			sphere->Scale(1.2f);
			Upgrade += Upgrade;
			Sphereradius *= 1.2;
			sphere->MoveY(5);
		}
		if (myEngine->KeyHit(Key_P) && !(GameState == Finished)) { // FIX LATER, UNNECESSARY
			if (PlayingState == Playing) {
				kSphereSpeed = 0;
				outText << "Pause";
				myFont->Draw(outText.str(), 560.0f, 335.0f, kBlue);
				PlayingState = Paused;
			}
			else {
				kSphereSpeed = 70.0f;
				PlayingState = Playing;
			}
		}

	}
	myEngine->Delete();
}
