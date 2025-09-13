/********************************************************************************************************************************************************
*
*   Frax Framework v2.0 - A simple and easy-to-use game emgine.
*
*   FEATURES:
*       - NO forced external dependencies, expect raylib.
*       - Multiplatform: My guess is that its just as good as raylib.
*
*   NOTES:
*		- In just a SINGLE file before including #define FRAX_IML
*		- Macros are prefixed with FRAX
* 		
*   DEPENDENCIES:
*       [raylib] for almost everything. 
*
*   OPTIONAL DEPENDENCIES:
*		[raygui] for GuiWIndow
* 	
*   LICENSE: MIT
*
* 	MIT License
*
* 	Copyright (c) 2025 Cube Nerd (@mastercuber55)
* 
* 	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation 
* 	files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, 
* 	modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software 
* 	is furnished to do so, subject to the following conditions:
* 
* 	The above copyright notice, this permission notice shall be included in all copies or substantial portions of the Software.
* 
* 	THE SOFTWARE IS PROVIDED "AS IS," WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
* 	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS 
* 	BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE, ARISING FROM, OUT 
* 	OF, OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
* 
********************************************************************************************************************************************************/

#ifndef FRAX_FRAMEWORK
#define FRAX_FRAMEWORK "2.0"

#ifndef FRAX_SCENE_DEFAULT_BACKGROUND_COLOR
#	define FRAX_SCENE_DEFAULT_BACKGROUND_COLOR BLACK
#endif

#ifndef FRAX_DEFAULT_FPS
#	define FRAX_DEFAULT_FPS 60
#endif

#include <raylib.h>

#ifdef FRAX_RAYGUI
#	ifdef FRAX_IMPL
#		define RAYGUI_IMPLEMENTATION
#	endif
#	include <raygui.h>
#	include <functional>
#endif // FRAX_RAYGUI

#include <string>

namespace Frax {
 
//----------------------------------------------------------------------------------
// Structures Definitions (Module: Structures)
//----------------------------------------------------------------------------------

// Scene, Used for scene management.
struct Scene {

	Color BackgroundColor;
	bool KeepRunning;
	int CodeToReturn;

	Scene(Color BackgroundColor = FRAX_SCENE_DEFAULT_BACKGROUND_COLOR);
	virtual bool ShouldClose();
	virtual int Run();
	virtual void Close(int CodeToReturn = 0);
	virtual void Update();
	virtual void Draw();

	~Scene();
};

// Rect, Used for easier rectangle manipulation
struct Rect {
		
	// Variables
	float x, y, w, h;								
	double Rotation;

	std::string TextureFile;						
	Rectangle Source;								
	Color Tint;										

	// De-Constructers
	Rect(float x, float y, float w, float h);
	Rect(Rectangle Destination, Color Tint);	
	Rect(										
		Rectangle Destination, 
		std::string TextureFile, 
		Rectangle Source = { 0, 0, -1, -1 }
	);
	~Rect();

	// Setters
	void operator=(Vector2 NewPosition);
	void SetTextureFile(std::string TextureFile); 

	// Getters
	Vector2 GetCenter();						
	operator Vector2() const;						
	operator Rectangle() const;						

	// Utilities
	bool IsColliding(Rect * Other);
	void Draw();									

};

#ifdef FRAX_RAYGUI
struct GuiWindow {

	bool Hidden;

	float x, y, w, h;
	std::string Title;

	GuiWindow(Rectangle Destination, std::string Title);

	void Update();
	void Draw(std::function<void(Vector2 Offset)> Function);
	
	private: bool Drag;
};
#endif // FRAX_RAYGUI

//------------------------------------------------------------------------------------
// Initialization And Closing Functions (Module: Core)
//------------------------------------------------------------------------------------
//
void Init(int WindowWidth, int WindowHeight, std::string WindowTitle);
void Close();
 
//------------------------------------------------------------------------------------
// Tool Functions (Module: Tools)
//------------------------------------------------------------------------------------
	
void WASDMovement(Rect * Object, float Speed);
bool AreColorSame(Color A, Color B);
Vector2 GetRandomPosition(Camera2D Cam);

}
#endif // FRAX_FRAMEWORK
//------------------------------------------------------------------------------------
// FRAX Implementation (Module: Implementation)
// No need to go any further, you know.
//------------------------------------------------------------------------------------

#ifdef FRAX_IMPL
#undef FRAX_IMPL

#include <unordered_map>
#include <algorithm>

//------------------------------------------------------------------------------------
// Variables (Module: Variables)
//------------------------------------------------------------------------------------

namespace Frax {

std::unordered_map<std::string, Texture> Textures;

//----------------------------------------------------------------------------------
// Structures Definitions (Module: Structures)
//----------------------------------------------------------------------------------

Scene::Scene(Color BackgroundColor) {
	this->BackgroundColor = BackgroundColor;
	this->CodeToReturn = 0;
	this->KeepRunning = true;
}
void Scene::Update() {}
void Scene::Draw() {}
void Scene::Close(int CodeToReturn) {
	this->KeepRunning = false;
	this->CodeToReturn = CodeToReturn;
}
int Scene::Run() {

	while(!this->ShouldClose()) {
		
		//------------------------------------------------------------------------------------
		// Event And Logic Handling
		//------------------------------------------------------------------------------------
		this->Update();
		//------------------------------------------------------------------------------------
		//  Drawing
		//------------------------------------------------------------------------------------		
		BeginDrawing();
			ClearBackground(this->BackgroundColor);
			this->Draw();
		EndDrawing();
	}
	return this->CodeToReturn;
}
bool Scene::ShouldClose() { 
	return !KeepRunning || WindowShouldClose();
}
Scene::~Scene() {}

Rect::Rect(float x, float y, float w, float h) {
	this->x = x, this->y = y;
	this->w = w, this->h = h;
	this->Tint = WHITE;
	this->Source = { 0, 0, -1, -1 },
	this->Rotation = 0.0;
}

Rect::Rect(Rectangle Rec, Color Tint) : Rect(Rec.x, Rec.y, Rec.width, Rec.height) {
	this->Tint = Tint;
}
Rect::Rect(Rectangle Destination, std::string TextureFile, Rectangle Source) : Rect(Destination, WHITE) {
	this->Source = Source;
	this->SetTextureFile(TextureFile);
}
Rect::~Rect() {}
void Rect::SetTextureFile(std::string TextureFile) {

	this->TextureFile = TextureFile; 						
	if(Textures.find(TextureFile) != Textures.end()) return;
	Textures[TextureFile] = LoadTexture(TextureFile.c_str());

	if(
		this->Source.x == 0 && 
		this->Source.y == 0 && 
		this->Source.width == -1 && 
		this->Source.height == -1
	) {
		this->Source.width = Textures[TextureFile].width, 
		this->Source.height = Textures[TextureFile].height;
	} 

}
void Rect::Draw() {

	(this->TextureFile.empty()) ? 
		DrawRectangle(this->x, this->y, this->w, this->h, this->Tint)
	:	
		DrawTexturePro(
			Textures[this->TextureFile], 
			this->Source, 
			Rectangle{ this->x + this->w / 2.0f, this->y + this->h / 2.0f, this->w, this->h }, 
			Vector2{ this->w / 2.0f, this->h / 2.0f }, 
			this->Rotation, 
			this->Tint
		);
}
bool Rect::IsColliding(Rect * Other) {
	return CheckCollisionRecs(*this, *Other);
}
void Rect::operator=(Vector2 NewPosition) {
	this->SetPosition(NewPosition);
}
Vector2 Rect::GetCenter() {
	return {this->x + this->w / 2.0f, this->y + this->h / 2.0f};
}
Rect::operator Vector2() const {
	return { this->x, this->y };
}
Rect::operator Rectangle() const {
	return { this->x, this->y, this->w, this->h };
}

#ifdef RAYGUI_H

GuiWindow::GuiWindow(Rectangle Destination, std::string Title) {
	this->x = Destination.x,
	this->y = Destination.y,
	this->w = Destination.width,
	this->h = Destination.height;

	this->Title = Title;

	this->Hidden = true;
	this->Drag = false;
}

void GuiWindow::Update() {
	if(this->Hidden) return;
	if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !Drag && CheckCollisionPointRec(GetMousePosition(), { x, y, w, 20.0f })) Drag = true;
	if(Drag) {

		Vector2 MouseDelta = GetMouseDelta();

		this->x += MouseDelta.x;
		this->y += MouseDelta.y;

		if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) Drag = false;
	}
}

void GuiWindow::Draw(std::function<void(Vector2 Offset)> Function) {
	if(this->Hidden) return;
	this->Hidden = GuiWindowBox((Rectangle){ x, y, w, h }, Title.c_str());
	
	Function({ this->x, this->y + 20.0f });
}
#endif // RAYGUI_H

void Init(int WindowWidth, int WindowHeight, std::string Title) {

	TraceLog(LOG_INFO, "Initializing Frax %s", FRAX_FRAMEWORK); 

	InitWindow(WindowWidth, WindowHeight, Title.c_str());
	InitAudioDevice();				

	SetExitKey(KEY_NULL);							
	SetTargetFPS(FRAX_DEFAULT_FPS);					
}
void Close() {


	CloseAudioDevice();
	CloseWindow();

	TraceLog(LOG_INFO, "Frax closed successfully");
}

//------------------------------------------------------------------------------------
// Tool Functions (Module: Tools)
//------------------------------------------------------------------------------------

void WASDMovement(Rect * Object, float Speed) {
	if(IsKeyDown(KEY_W)) Object->y += -Speed;
	if(IsKeyDown(KEY_A)) Object->x += -Speed;
	if(IsKeyDown(KEY_S)) Object->y += +Speed;
	if(IsKeyDown(KEY_D)) Object->x += +Speed;
}

bool AreColorSame(Color A, Color B) {
	return ColorToInt(A) == ColorToInt(B);
}
Vector2 GetRandomPosition(Camera2D Cam) {
	return {
		(float)(GetRandomValue(Cam.target.x - Cam.offset.x, Cam.target.x + Cam.offset.x)),
		(float)(GetRandomValue(Cam.target.y - Cam.offset.y, Cam.target.y + Cam.offset.y))
	};
}

}

#endif // FRAX_IMPL