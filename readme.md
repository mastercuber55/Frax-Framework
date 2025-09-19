
# Frax Framework

Frax, a header only 2D easy-to-use game framework for providing easier and structural way to use raylib.


## Features

- Easy To Use
- Header Only
- Customization
- Only [raylib](https://raylib.com) required
- Easier Rectangle Manipulation
- Sprite Management

### Upcoming Features

- Network Management
- Audio Management

## Installation & Configuration
The recommended way to add Frax to your project is the following. However, if you wish to you can just drop the `Frax.hpp` file onto your project.

`git submodule add https://github.com/mastercuber55/Frax-Framework.git external/FraxFramework`

Then, in just a SINGLE file( usually just main.cxx ) before including `#define FRAX_IML`
Here are also some customizations you can do.

`define FRAX_SCENE_DEFAULT_BACKGROUND_COLOR` - Defaults to `BLACK`
`define FRAX_DEFAULT_FPS` - Defaults to `60`

## Documentation
Everything is plain and simple so just look into the header's top part and you'll understand everything.

This is the recommended way to structure your project.
(Project Name)
├── Assets
│   ├── Board.gif
│   └── Marks.png
├── CMakeLists.txt
├── external
│   └── FraxFramework
│       └── Frax.hpp
├── main.cxx
└── Scenes
    ├── SceneTitle.cxx
    ├── SceneGame.cxx
    └── Scenes.hpp

### Making A Scene
```cxx
// Scenes/Scenes.hpp
struct SceneGame : Frax::Scene {
    
    // Anything you need for your game.
    Sound shoot;
    Camera2D Cam;
    Frax::Rect Player;

    SceneGame();

    void Update(float dt) override;
    void Draw() override;
    
    ~SceneGame();
};
```
```cxx
// Scenes/SceneGame.cxx
#include "Scenes.hpp"

SceneGame::SceneGame() : Player({0, 0, 32, 32}, "Assets/Player.png") {
    //Initialize anything needed for the game
}

void SceneGame::Update(float dt) {
    // Handle inputs, logics, events
}

void SceneGame::Draw() {
    
    // Before cam layer, like background and stuff.

    BeginMode2D(Cam);
        // Draw stuff with the camera.
        Player.Draw();
    EndMode2D();

    // After cam layer, like UI and sutff
}

SceneGame::~SceneGame() {
    // De init anything if needed.
}
```
## License

[MIT](https://choosealicense.com/licenses/mit/)


## Usage/Examples

- [Physics Example](https://github.com/mastercuber55/Physics-Example)
- [Snake Game](https://github.com/mastercuber55/Snake-Game)

