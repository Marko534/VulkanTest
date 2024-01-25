#pragma once

#include "lve_window.hpp"
#include "lve_device.hpp"
#include "lve_game.hpp"
#include "lve_renderer.hpp"
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