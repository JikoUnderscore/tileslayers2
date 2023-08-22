# tileslayers2

### **vspkg linking for SDL required**
run `vcpkg install sdl2:x64-windows-static sdl2-image:x64-windows-static`   
OR 
run `vcpkg install sdl2:x64-windows sdl2-image:x64-windows`   


### **premake required**
run `premake5 vs2022`


In `Configuration Properties` -> `VC++ Directories` Set `C++ Language Standard` to `/stdc++latest` (c++23 needed) for All Configurations

[OPTIONAL] In `Configuration Properties` -> `vcpkg` Set `Use Static Libraries` to `Yes`   


In `Configuration Properties` -> `vcpkg` Write in `Triplet`:  `x64-windows`