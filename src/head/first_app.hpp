#pragma once

#include "head/lve_window.hpp"
#include "head/lve_device.hpp"
#include "head/lve_game_object.hpp"
#include "head/lve_renderer.hpp"

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
    void addVase(glm::vec3 color = glm::vec3{1.f},
                 glm::vec3 position = glm::vec3{0.f},
                 glm::vec3 rotation = glm::vec3{0.f},
                 glm::vec3 scale = glm::vec3{1.f});
    LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
    LveDevice lveDevice{lveWindow};
    LveRenderer lveRenderer{lveWindow, lveDevice};

    std::vector<LveGameObject> gameObjects;
  };
}