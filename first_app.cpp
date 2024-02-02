#include "first_app.hpp"
#include "keyboard_movement_controller.hpp"
#include "simple_render_system.hpp"
// #include "lve_camera.hpp"

// libs

#define GLM_FORCE_RADIANS
// May want to delete to make it simmilar to openGl
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <stdexcept>
#include <chrono>
#include <array>
#include <cassert>
#include <iostream>

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
    LveCamera camera{};

    auto viewerObject = LveGameObject::createGameObject();
    viewerObject.transform.translation = {0.f, 0.f, 10.f};
    viewerObject.transform.rotation = {0.f, glm::pi<float>(), 0.f};

    KeyboardMovmentController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!lveWindow.shouldClose())
    {
      glfwPollEvents();

      auto newTime = std::chrono::high_resolution_clock::now();
      float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
      currentTime = newTime;

      // frameTime = glm::min(MAX_FRAME_TIME);

      cameraController.moveInOrbit(lveWindow.getGLFWwindow(), frameTime, viewerObject);
      // cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
      // camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);
      camera.setViewDirection(viewerObject.transform.translation, -viewerObject.transform.translation);

      // DEBUG POSITION
      // std::cout
      //     << "Transform"
      //     << "\t x: " << viewerObject.transform.translation.x << "\t y: " << viewerObject.transform.translation.y << "\t z: " << viewerObject.transform.translation.z << "\t";
      // std::cout << "Rotation"
      //           << "\t x: " << viewerObject.transform.rotation.x << "\t y: " << viewerObject.transform.rotation.y << "\t z: " << viewerObject.transform.rotation.z << std::endl;

      float aspect = lveRenderer.getAspectRatio();
      camera.setPerspectiveProjection(glm::radians(45.f), aspect, 0.1, 100.f);

      if (auto commandBuffer = lveRenderer.beginFrame())
      {

        lveRenderer.beginSwapChainRenderPass(commandBuffer);
        simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
        lveRenderer.endSwapChainRenderPass(commandBuffer);
        lveRenderer.endFrame();
      }
    }

    vkDeviceWaitIdle(lveDevice.device());
  }

  // temporary helper function, creates a 1x1x1 cube centered at offset with a index budder.
  std::unique_ptr<LveModel> createCubeModel(LveDevice &device, glm::vec3 offset)
  {
    LveModel::Builder modelBuilder{};
    modelBuilder.vertices = {
        // left face (red) 1.f, 0.f, 0.f
        {{-.5f, -.5f, -.5f}, {1.f, 0.f, 0.f}},
        {{-.5f, .5f, .5f}, {1.f, 0.f, 0.f}},
        {{-.5f, -.5f, .5f}, {1.f, 0.f, 0.f}},
        {{-.5f, .5f, -.5f}, {1.f, 0.f, 0.f}},

        // right face (orange) 0.8f, 0.31f, 0.f
        {{.5f, -.5f, -.5f}, {0.8f, 0.31f, 0.f}},
        {{.5f, .5f, .5f}, {0.8f, 0.31f, 0.f}},
        {{.5f, -.5f, .5f}, {0.8f, 0.31f, 0.f}},
        {{.5f, .5f, -.5f}, {0.8f, 0.31f, 0.f}},

        // top face (yellow, remember y axis points down) 1.f, 1.f, 0.f
        {{-.5f, -.5f, -.5f}, {1.f, 1.f, 0.f}},
        {{.5f, -.5f, .5f}, {1.f, 1.f, 0.f}},
        {{-.5f, -.5f, .5f}, {1.f, 1.f, 0.f}},
        {{.5f, -.5f, -.5f}, {1.f, 1.f, 0.f}},

        // bottom face (white) 1.f, 1.f, 1.f
        {{-.5f, .5f, -.5f}, {1.f, 1.f, 1.f}},
        {{.5f, .5f, .5f}, {1.f, 1.f, 1.f}},
        {{-.5f, .5f, .5f}, {1.f, 1.f, 1.f}},
        {{.5f, .5f, -.5f}, {1.f, 1.f, 1.f}},

        // nose face (blue) 0.f, 0.f, 1.f
        {{-.5f, -.5f, 0.5f}, {0.f, 0.f, 1.f}},
        {{.5f, .5f, 0.5f}, {0.f, 0.f, 1.f}},
        {{-.5f, .5f, 0.5f}, {0.f, 0.f, 1.f}},
        {{.5f, -.5f, 0.5f}, {0.f, 0.f, 1.f}},

        // tail face (green) 0.f, 0.5f, 0.f
        {{-.5f, -.5f, -0.5f}, {0.f, 0.5f, 0.f}},
        {{.5f, .5f, -0.5f}, {0.f, 0.5f, 0.f}},
        {{-.5f, .5f, -0.5f}, {0.f, 0.5f, 0.f}},
        {{.5f, -.5f, -0.5f}, {0.f, 0.5f, 0.f}},

    };
    for (auto &v : modelBuilder.vertices)
    {
      v.position += offset;
    }
    modelBuilder.indices = {0, 1, 2, 0, 3, 1, 4, 5, 6, 4, 7, 5, 8, 9, 10, 8, 11, 9,
                            12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};

    return std::make_unique<LveModel>(device, modelBuilder);
  }

  // YOU MAKE THE MODELS HERE
  void FirstApp::loadGameObjects()
  {
    std::shared_ptr<LveModel> lveModel1 = createCubeModel(lveDevice, glm::vec3{0.f});
    std::shared_ptr<LveModel> lveModel2 = LveModel::createModelFromFile(lveDevice, "models/smooth_vase.obj", {1.f, 0.f, 0.69f});
    std::shared_ptr<LveModel> lveModelArrowX = LveModel::createModelFromFile(lveDevice, "models/arrow.obj", {1.f, 0.f, 0.f});
    std::shared_ptr<LveModel> lveModelArrowY = LveModel::createModelFromFile(lveDevice, "models/arrow.obj", {0.f, 1.f, 0.f});
    std::shared_ptr<LveModel> lveModelArrowZ = LveModel::createModelFromFile(lveDevice, "models/arrow.obj", {0.f, 0.f, 1.f});

    auto arrowX = LveGameObject::createGameObject();
    auto arrowY = LveGameObject::createGameObject();
    auto arrowZ = LveGameObject::createGameObject();

    arrowX.model = lveModelArrowX;
    arrowY.model = lveModelArrowY;
    arrowZ.model = lveModelArrowZ;

    // arrowZ.transform.rotation = {glm::pi<float>(), 0.f, 0.f};

    arrowX.transform.translation = {0.75f, 0.f, 0.f};
    arrowY.transform.translation = {0.f, 0.75f, 0.f};
    arrowZ.transform.translation = {0.f, 0.f, 0.75f};

    arrowX.transform.rotation = {0.f, 0.f, glm::three_over_two_pi<float>()};
    arrowZ.transform.rotation = {glm::half_pi<float>(), 0.f, 0.f};

    // cube.transform.scale = {0.5f, 0.5f, 0.5f};
    gameObjects.push_back(std::move(arrowX));
    gameObjects.push_back(std::move(arrowY));
    gameObjects.push_back(std::move(arrowZ));

    auto cube1 = LveGameObject::createGameObject();
    auto cube2 = LveGameObject::createGameObject();

    cube1.model = lveModel1;
    cube2.model = lveModel2;

    cube2.transform.translation = {0.f, 0.f, 2.5f};

    // gameObjects.push_back(std::move(cube1));
    gameObjects.push_back(std::move(cube2));
  }

}