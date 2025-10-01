# Frax Framework

**Frax** is a lightweight, header-only 2D game framework built on top of [raylib](https://raylib.com).
It’s designed to give you a more structured, easier, and customizable way to build games without the usual boilerplate.

---

## ✨ Features

* **Header-only** → just drop in and use
* **Zero dependencies** (only raylib required)
* **Easy-to-use API** for scenes, sprites, and rectangles
* **Automatic sprite & texture management**
* **Simple rectangle manipulation utilities**
* **Highly customizable**

---

## 🚧 Upcoming Features

* Network management
* Audio management

---

## ⚙️ Installation & Configuration

The recommended way to add Frax to your project:

```sh
git submodule add https://github.com/mastercuber55/Frax-Framework.git external/FraxFramework
```

In your main file (e.g. `main.cxx`), define `FRAX_IML` **before** including the header:

```cxx
#define FRAX_IML
#include "Frax.hpp"
```

### Customization Macros

* `FRAX_SCENE_DEFAULT_BACKGROUND_COLOR` → Default is `BLACK`

---

## 📖 Documentation

Everything is contained in a single header (`Frax.hpp`). The code is straightforward, and the top section of the file explains most things.

### Suggested Project Structure

```
(ProjectName)
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
```

---

## 🎬 Creating a Scene

**Header (Scenes/Scenes.hpp):**

```cxx
struct SceneGame : Frax::Scene {
    Sound shoot;
    Camera2D Cam;
    Frax::Rect Player;

    SceneGame();
    void Update(float dt) override;
    void Draw() override;
    ~SceneGame();
};
```

**Implementation (Scenes/SceneGame.cxx):**

```cxx
#include "Scenes.hpp"

SceneGame::SceneGame() : Player({0, 0, 32, 32}, "Assets/Player.png") {
    // Initialize game resources
}

void SceneGame::Update(float dt) {
    // Handle input, logic, events
}

void SceneGame::Draw() {
    // Pre-camera layer (backgrounds, etc.)

    BeginMode2D(Cam);
        Player.Draw();
    EndMode2D();

    // Post-camera layer (UI, overlays, etc.)
}

SceneGame::~SceneGame() {
    // Cleanup if needed
}
```

---

## 🎨 Sprites & Rectangles

Managing textures, positions, sizes, and rotations manually can get messy. Frax provides `Rect` for a simple, ready-to-use sprite entity:

```cxx
struct Rect {
    // Variables
    float x, y, w, h;
    double Rotation;
    std::shared_ptr<Texture> TexturePtr;
    Rectangle Source;
    Color Tint;
    std::any Data;

    // Constructors
    Rect(float x, float y, float w, float h, Color tint = WHITE);
    Rect(Rectangle dest, Color tint = WHITE);
    Rect(Rectangle dest, const std::string &textureFileStr,
         Rectangle Source = {0, 0, -1, -1});
    ~Rect();

    // Setters
    void SetCenter(Vector2 cenpos);
    void operator=(Vector2 pos);
    void SetTextureFile(const std::string &textureFileStr);

    // Getters
    Vector2 GetCenter();
    operator Vector2() const;
    operator Rectangle() const;

    // Utilities
    void Draw();
};
```

* All fields are **public** for easy direct access.
* `TexturePtr` is automatically managed by Frax’s internal sprite manager.
* `Data` allows you to attach arbitrary data (e.g., a physics body).

### Example: Extending Rect

For more complex entities, inherit `Rect` and add your own variables:

```cxx
struct Entity : Frax::Rect {
    float Health;
    Pebble::Obj* Phy;

    using Rect::Rect; // inherit all constructors

    void Draw();
};
```

Here, we attach a physics body (via [Pebble](https://github.com/mastercuber55/pebble)) and a health variable.

> 📝 **Tip:**
> Use `using Rect::Rect` to inherit constructors. Define custom constructors if needed.

---

## 🧰 Recommended Physics

* [Chipmunk Physics](https://chipmunk-physics.net/)
* [Pebble](https://github.com/mastercuber55/pebble) (wrapper for Chipmunk)
* [Box2D](https://box2d.org/)

---

## 📜 License

[MIT](https://choosealicense.com/licenses/mit/)

---