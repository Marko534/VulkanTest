# BlenderLikeProgram (with vulkan)

## Installing the necessary packages (Linux)
Made this project primarily in Fedora, so I am most familiar with dnf.

Here is a link to set up your [environment](https://vulkan-tutorial.com/Development_environment#page_Vulkan-Packages) 
(tldr bellow)

### Vulkan
apt
```bash
sudo apt install vulkan-tools
sudo apt install libvulkan-dev
sudo apt install vulkan-validationlayers-dev spirv-tools 
``` 
rpm
```bash
sudo dnf install vulkan-tools
sudo dnf install vulkan-loader-devel 
sudo dnf install mesa-vulkan-devel vulkan-validation-layers-devel
``` 
Arch
HAHAHA no.

### GLFW
apt
```bash
sudo apt install libglfw3-dev
``` 
rpm
```bash
sudo dnf install glfw-devel
``` 
Arch
HAHAHA no.
### GLM
apt
```bash
sudo apt install libglm-dev
``` 
rpm
```bash
sudo dnf install glm-devel
``` 
Arch
HAHAHA no.

### Shaders
apt

download this [link](https://github.com/google/shaderc/blob/main/downloads.md)

Two popular shader compilers are Khronos Group's glslangValidator and Google's glslc. The latter has a familiar GCC- and Clang-like usage, so we'll go with that: on Ubuntu, download Google's unofficial binaries and copy glslc to your /usr/local/bin. Note you may need to sudo depending on your permissions.

rpm
```bash
sudo dnf install glslc
``` 
Arch
HAHAHA no.
### Misc

apt
```bash
sudo apt install libxxf86vm-dev libxi-dev
``` 
rpm
```bash
sudo dnf install libXi-devel libXxf86vm-devel
sudo dnf group install "Development Tools"
``` 
Arch
HAHAHA no.

## Building and running the project
The project is made with Cmake. You just need to run one script.
```bash
chmod +x unixBuild.sh
./unixBuild.sh
``` 

If you are having problems try to delete the build folder.
