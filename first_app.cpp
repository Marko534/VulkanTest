#include "first_app.hpp"
#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
// May want to delete to make it simmilar to openGl
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <stdexcept>
#include <array>
#include <cassert>

// AAAAAAAA
#include <glm/gtc/constants.hpp>

namespace lve
{
  FirstApp::FirstApp()
  {
    loadGameObjects();
  }

  FirstApp::~FirstApp() {}

  void FirstApp::run()
  {
    SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};

    while (!lveWindow.shouldClose())
    {
      glfwPollEvents();

      if (auto commandBuffer = lveRenderer.beginFrame())
      {

        lveRenderer.beginSwapChainRenderPass(commandBuffer);
        simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
        lveRenderer.endSwapChainRenderPass(commandBuffer);
        lveRenderer.endFrame();
      }
    }

    vkDeviceWaitIdle(lveDevice.device());
  }

  // temporary helper function, creates a 1x1x1 cube centered at offset
  std::unique_ptr<LveModel> createCubeModel(LveDevice &device, glm::vec3 offset)
  {
    std::vector<LveModel::Vertex> vertices{

        // left face (white)
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

        // right face (yellow)
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

        // top face (orange, remember y axis points down)
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

        // bottom face (red)
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

        // nose face (blue)
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

        // tail face (green)
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

    };
    for (auto &v : vertices)
    {
      v.position += offset;
    }
    return std::make_unique<LveModel>(device, vertices);
  }

  // YOU MAKE THE MODELS HERE
  void FirstApp::loadGameObjects()
  {
    std::shared_ptr<LveModel> lveModel = createCubeModel(lveDevice, {0.f, 0.f, 0.f});

    auto cube = LveGameObject::createGameObject();
    cube.model = lveModel;
    cube.transform.translation = {0.f, 0.f, 0.5f};
    cube.transform.scale = {0.5f, 0.5f, 0.5f};
    gameObjects.push_back(std::move(cube));
  }

}