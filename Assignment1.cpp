
#include <sstream>
#include <TL-Engine.h>	
using namespace tle;
using namespace std;

void main()
{
	float kSphereSpeed = 0.1f;
	const float kRotationSpeed = 0.05f;
	bool pPressed = false;
	bool CameraPressed = false;
	bool canDraw = false;
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	float Sphereradius = 10.0;
	const float CubeSides = 5.0;
	
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	IFont* myFont = myEngine->LoadFont("Comic Sans MS", 36);
	float fontX = 0;
	float fontY = 0;


	
	int playerPoints = 0;
	int Upgrade = 40;
	
	IMesh* waterMesh = myEngine->LoadMesh("water.x");
	IModel* water = waterMesh->CreateModel(0,-5,0);

	IMesh* islandMesh = myEngine->LoadMesh("island.x");
	IModel* island = islandMesh->CreateModel(0, -5, 0);

	IMesh* sphereMesh = myEngine->LoadMesh("sphere.x");
	IModel* sphere = sphereMesh->CreateModel(0, 10, 0);

	IMesh* cubeMesh = myEngine->LoadMesh("minicube.x");
	/*IModel* cube1 = cubeMesh->CreateModel(-80, 0, 80);
	IModel* cube2 = cubeMesh->CreateModel(80,0,80);
	IModel* cube3 = cubeMesh->CreateModel(80,0,-80);
	IModel* cube4 = cubeMesh->CreateModel(-80,0,-80);*/

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

	while (myEngine->IsRunning())
	{
		myEngine->DrawScene();

		if (myEngine->KeyHit(Key_Escape)) {
			break;
		}
		if (myEngine->KeyHeld(Key_W)) {
			sphere->MoveZ(kSphereSpeed);
		}
		if (myEngine->KeyHeld(Key_S)) {
			sphere->MoveZ(-kSphereSpeed);
		}
		if (myEngine->KeyHeld(Key_A)) {
			sphere->MoveX(-kSphereSpeed);
		}
		if (myEngine->KeyHeld(Key_D)) {
			sphere->MoveX(kSphereSpeed);
		}

		if (myEngine->KeyHit(Key_P)) {
			if (pPressed == false) {
				kSphereSpeed = 0;
				pPressed = true;
			}
			else {
				kSphereSpeed = 0.05f;
				pPressed = false;
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
		/*float cubex, cubey, cubez;
		cubex = cube1->GetX() - sphere->GetX();
		cubey = cube1->GetY() - sphere->GetY();
		cubez = cube1->GetZ() - sphere->GetZ();
		float path1 = sqrt(cubex * cubex + cubey * cubey + cubez * cubez);

		float cubex2, cubey2, cubez2;
		cubex2 = cube2->GetX() - sphere->GetX();
		cubey2 = cube2->GetY() - sphere->GetY();
		cubez2 = cube2->GetZ() - sphere->GetZ();
		float path2 = sqrt(cubex2 * cubex2 + cubey2 * cubey2 + cubez2 * cubez2);

		float cubex3, cubey3, cubez3;
		cubex3 = cube3->GetX() - sphere->GetX();
		cubey3 = cube3->GetY() - sphere->GetY();
		cubez3 = cube3->GetZ() - sphere->GetZ();
		float path3 = sqrt(cubex3 * cubex3 + cubey3 * cubey3 + cubez3 * cubez3);

		float cubex4, cubey4, cubez4;
		cubex4 = cube4->GetX() - sphere->GetX();
		cubey4 = cube4->GetY() - sphere->GetY();
		cubez4 = cube4->GetZ() - sphere->GetZ();
		float path4 = sqrt(cubex4 * cubex4 + cubey4 * cubey4 + cubez4 * cubez4);

		if (path1 < Sphereradius + CubeSides / 2) {
			cube1->SetPosition(0, -200, 0);
			playerPoints += 10;
			
		}
		if (path2 < Sphereradius + CubeSides / 2) {
			cube2->SetPosition(0, -200, 0);
			playerPoints += 10;
			
		}
		if (path3 < Sphereradius + CubeSides / 2) {
			cube3->SetPosition(0, -200, 0);
			playerPoints += 10;
			
		}
		if (path4 < Sphereradius + CubeSides / 2) {
			cube4->SetPosition(0, -200, 0);
			playerPoints += 10;
			
		} */


		stringstream outText;
		outText << "Points: " << playerPoints;
		myFont->Draw(outText.str(), fontX, fontY, kBlack);
		if (playerPoints >= Upgrade) {
			sphere->Scale(1.2f);
			Upgrade += Upgrade;
			Sphereradius *= 1.2;
			sphere->MoveY(5);
		}
		

		if (myEngine->KeyHit(Key_2)) {
			if (CameraPressed == false) {
				ICamera* camera = myEngine->CreateCamera(kManual, 150, 150, -150);
				camera->RotateLocalY(-45);
				camera->RotateLocalX(45);

				CameraPressed = true;
			}

		}
		if (myEngine->KeyHit(Key_1)) {
			if (CameraPressed == true) {
				ICamera* camera = myEngine->CreateCamera(kManual, 0, 200, 0);
					camera->RotateLocalX(90);

					CameraPressed = false;
			}
		}









	}
	myEngine->Delete();
}
