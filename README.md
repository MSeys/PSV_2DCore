# PSV_2DCore
### **Version 1.0 (Released August 12 - 2019)**

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

The user is only needed to use:
```cpp 
void PSV_SetTouchSamplingMode(const PSV_TouchSamplingMode& psvTouchSamplingMode);
```

You can switch between checking motion or swiping using this function.
This is the only PSV function that should be called by the user as the others are called inside the Core.

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
* Draw Point (changeable point size)
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

## Wiki
A wiki will be in the works soon that should include better information than this README and should be more clear.

## Samples
The samples will be found in their dedicated github repository. 
These will include their GameHandler files and then specific classes for their sample.
This will require EVERY sample to work however as you will be able to test them all with compiling just once!

[Samples](https://github.com/MSeys/PSV_2DCore_samples)

## Libraries
* Vita2d
* SoLoud
* ...
(They can be found in CMakeLists.txt)
