CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTest: *.cpp *.hpp
	g++ $(CFLAGS) -o VulkanTest *.cpp $(LDFLAGS)

.PHONY: test clean

discrete: VulkanTest	 
	__NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia ./VulkanTest

integrated: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest ma