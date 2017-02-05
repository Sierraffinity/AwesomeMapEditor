# Awesome Map Editor
Awesome Map Editor (AME) is a map editor for 3rd generation Pokémon games: Ruby, Sapphire, FireRed, LeafGreen, and Emerald.

## Dependencies
[QBoy](https://github.com/pokedude9/QBoy)  
[yaml-cpp](https://github.com/jbeder/yaml-cpp)

## Getting Started
1. Place the dependencies in the same directory as the AME root.

2. Open all the projects in Qt and set their build directories to `bin`. For yaml-cpp, open the `CMakeLists.txt` file and ensure BUILD_SHARED_LIBS is enabled in its project settings.

3. Build them all, working forwards from yaml-cpp to QBoy to AME itself.

4. Copy the `resources/config` folder into the AME executable directory.

Now you can help us develop the future of Pokémon ROM hacking!

## Credits
**Diegoisawesome** - Developer

**Pokedude** - Developer
