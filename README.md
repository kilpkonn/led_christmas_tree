# LED christmas tree

TODO: pictures

## How to run:

### Arduino IDE
Just copy paste `led_christmas_tree.cpp` content to your `.ino` file.  
Should be good to go.  

### CLion
1. Clone repository.
```bash
git clone https://github.com/kilpkonn/led_christmas_tree.git
```
2. Install Arduino support plugin. (https://plugins.jetbrains.com/plugin/11301-arduino-support)
```
File > Settings > Plugins > Marketplace > Search Arduino Support
```
3. Make new Arduino sketch project.
```
File > New Project > Arduino Sketch
```
4. Copy `cmake` and `CMakeLists.txt` to cloned project
```bash
cp -a /path/to/cmake /path/to/cloned/repository/root
cp /path/to/CMakeLists.txt /path/to/cloned/repository/root
```
You can now close sketch project you created, no need to save.  

5. Edit CMakeLists.txt  
    1. I had to remove `<@` and `@>` so it looked like `set(${PROJECT_NAME}_BOARD nano)`  
    2. Add `FastLED` library.
    3. make sure sketch entry point is set `set(${PROJECT_NAME}_SKETCH src/led_christmas_tree.cpp)`
