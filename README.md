# tileslayers2

### **vspkg linking for SDL required**
### **premake required**

run `premake vs2022`

In `Configuration Properties` -> `VC++ Directories` Set `C++ Language Standard` to `/stdc++latest` (c++23 needed) for All Configurations

In `Configuration Properties` -> `vcpkg` Set `Use Static Libraries` to `Yes`   
Note: make non-static optional. It dose not work for me, maybe it works for you?


In `Configuration Properties` -> `vcpkg` Write in `Triplet`:  `x64-windows`