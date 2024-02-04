#pragma once

#include "head/lve_window.hpp"
#include "head/lve_device.hpp"
#include "head/lve_game_object.hpp"
#include "head/lve_renderer.hpp"
#include "head/UseImGui.hpp"

// sdt
#include <memory>
#include <vector>

namespace lve
{

  class FirstApp
  {

  public:
    static constexpr int WIDTH = 1024;
    static constexpr int HEIGHT = 1024;

    FirstApp();
    ~FirstApp();

    FirstApp(const FirstApp &) = delete;
    FirstApp &operator=(const FirstApp &) = delete;

    void run();

  private:
    void loadGameObjects();
    LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
    LveDevice lveDevice{lveWindow};
    LveRenderer lveRenderer{lveWindow, lveDevice};

    std::vector<LveGameObject> gameObjects;
  };
}