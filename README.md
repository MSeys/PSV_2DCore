# PSV_2DCore
### **Version 1.13 (Released September 9th - 2019)**

## Functionality
### PSV
#### Event loop - events
* Key Down
* Key Up
* Key Held
* Joystick Motion
* Touchpad Down
* Touchpad Up
* Touchpad Motion
* Touchpad Swipe

#### PSV Functions (used by user)

**Setting the Touch sampling mode**
```cpp 
void PSV_SetTouchSamplingMode(const PSV_TouchSamplingMode& psvTouchSamplingMode);
```

You can switch between checking motion or swiping using this function.
This is the only PSV function that should be called by the user as the others are called inside the Core.

**Using PSV Transformations**
```cpp
void PSV_Begin();
void PSV_Translate(float x, float y);
void PSV_Scale(float x, float y);
void PSV_End();
```

These functions can be used to transform your entire screen / camera / window, these apply on shapes, text/font and textures.
While they are in V1.1, they are NOT finished and require improvement to be able to have transformations inside transformations, currently they act standalone!

### Bank (Textures, Fonts, SFX, Music)
The bank is a special class that handles the files found in a folder structures pre-defined in pch.cpp.
(You can change these paths, but you can stick to the default ones aswell)

These handle the objects: 
* Texture (Differentiate between Game Assets and UI)
* Font
* SFX
* Music

You'll learn more about these by checking their files out.

### Drawing functionality (built on vita2d)
* Draw Line (changeable line thickness)
* Draw Rectangle
* Fill Rectangle
* Fill Circle
### Basic Collision functionality
* Point in Rectangle
* Point in Circle
* Point in Polygon
* Overlapping
  * Two Points and a Circle
  * Two Points and a Rectangle
  * Two Rectangles
  * A Rectangle and a Circle
  * Two Circles
  * Vertices and a Circle
* Raycast
* Intersect Line Segments
* Distance Point and Line Segment
* Point on Line Segment

### Other
* Get Time Now
* Distance between Two Points
* Angle between Two Points (in radians)
* Degrees to Radians
* Radians to Degrees
* Angle in Range (degrees given with other degree values as limiters)

## How to use
Copy Resources (remove README files), sce_sys, src and CMakeLists.txt

### GameHandler
In "src", you will find GameHandler.h and GameHandler.cpp.
This is where your "access" starts, here you'll find:

```cpp
// GameHandler.h

void Update(float elapsedSec);
void Draw() const;

void ProcessKeyUpEvent(const PSV_ButtonEvent& bEvent);
void ProcessKeyDownEvent(const PSV_ButtonEvent& bEvent);
void ProcessKeyHeldEvent(const PSV_ButtonEvent& bEvent);

void ProcessJoystickMotionEvent(const PSV_JoystickEvent& jEvent);

void ProcessTouchpadDownEvent(const PSV_TouchpadEvent& tpEvent);
void ProcessTouchpadUpEvent(const PSV_TouchpadEvent& tpEvent);
void ProcessTouchpadMotionEvent(const PSV_TouchpadEvent& tpEvent);
void ProcessTouchpadSwipeEvent(const PSV_TouchpadEvent& tpEvent);
	
void Initialize();
void Cleanup();
```
These are the functions you'll be using (and the constructor/destructor).

#### Draw
This function is used to Draw/Render your shapes, textures, ...

#### Update
Want to update a position by using the execution time/elapsed sec? (0.016~ when running at 60~ FPS)
Here is where you can do exactly that.
(For example moving a shape at a specific velocity with the elapsed seconds to move it inside boundaries)

#### Process...Event
These functions are where the events come in.
These are called when their event triggers (this is done behind code inside Core / PSV).

You dont have to check when the event should trigger, you can just use the events (parameter) data and start checking things out!

#### Initialize and Cleanup
These functions are called in the constructor and destructor respectively.
You don't have to work in the constructor or destructor as you can use these aswell.

#### Is this the only file I can work in?

No! You can create your own class, copy the necessary functions from GameHandler.h and call those inside GameHandler (by creating an object of your class inside GameHandler constructor)

## Wiki
A wiki will be in the works soon that should include better information than this README and should be more clear.

## Samples
Samples can be found here:
[Samples](https://github.com/MSeys/PSV_2DCore_samples)

Good luck!

## Extra Libraries besides vitasdk
* Vita2d
* SoLoud

Be sure to check CMakeLists.text to know all dependencies.

## Like what I do?
Maybe send a small donation to let me know that you appreciate the work!

[Paypal](https://www.paypal.me/mattseys)

# Questions or help needed?
* Contact me on [Twitter](https://twitter.com/seys_matthias).
