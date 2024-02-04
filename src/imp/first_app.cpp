#include "head/first_app.hpp"
#include "head/keyboard_movement_controller.hpp"
#include "head/simple_render_system.hpp"
#include "head/UseImGui.hpp"
// #include "head/UseImGui.hpp"

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    UseImGui myimgui{lveWindow.getGLFWwindow(), lveDevice, lveRenderer};

    auto currentTime = std::chrono::high_resolution_clock::now();

    // VASE
    static bool spawnVase = false;
    glm::vec3 positionVase{0.f};
    glm::vec3 scaleVase{1.f};
    glm::vec3 rotationVase{0.f};
    glm::vec4 colorVase(1.f);

    // CUBE
    static bool spawnCube = false;
    glm::vec3 positionCube{0.f};
    glm::vec3 scaleCube{1.f};
    glm::vec3 rotationCube{0.f};
    glm::vec4 colorCube(1.f);

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

      if (spawnVase)
      {
        addVase(glm::vec3{colorVase.x, colorVase.y, colorVase.z},
                positionVase,
                rotationVase,
                scaleVase);
        spawnVase = false;
      }
      if (spawnCube)
      {
        addCube(glm::vec3{colorCube.x, colorCube.y, colorCube.z},
                positionCube,
                rotationCube,
                scaleCube);
        spawnCube = false;
      }
      if (auto commandBuffer = lveRenderer.beginFrame())
      {

        lveRenderer.beginSwapChainRenderPass(commandBuffer);
        myimgui.NewFrame();
        {
          // ADD
          ImGui::Begin("ADD ENTITY");        // Create a window called "Hello, world!" and append into it.
          spawnVase = ImGui::Button("VASE"); // Edit bools storing our window open/close state
          spawnCube = ImGui::Button("CUBE"); // Edit bools storing our window open/close state
          ImGui::End();

          ImGui::Begin("Entity"); // Create a window called "Hello, world!" and append into it.

          for (auto &entity : gameObjects)
          {
            ImGui::NewLine();
            ImGui::InputText("##NameBuffer", entity.transform.name.data(), entity.transform.name.size());

            // static char str0[128] = "Hello, world!";
            // ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));
            glm::vec3 position = entity.transform.translation;
            ImGui::DragFloat3("POSITION", &position.x, 0.001f, -5.f, 5.f);
            entity.transform.translation = position;
            // ImGui::DragFloat3("SCALE", &scaleVase.x, 0.001f, -5.f, 5.f);
            // ImGui::DragFloat3("ROTATION (in rad)", &rotationVase.x, 0.001f, -5.f, 5.f);
          }
          ImGui::End();

          // VASE
          ImGui::Begin("VASE");                            // Create a window called "Hello, world!" and append into it.
          ImGui::ColorEdit3("COLOR", (float *)&colorVase); // Edit 3 floats representing a color
          ImGui::DragFloat3("POSITION", &positionVase.x, 0.001f, -5.f, 5.f);
          ImGui::DragFloat3("SCALE", &scaleVase.x, 0.001f, -5.f, 5.f);
          ImGui::DragFloat3("ROTATION (in rad)", &rotationVase.x, 0.001f, -5.f, 5.f);
          ImGui::End();

          // CUBE
          ImGui::Begin("CUBE");                            // Create a window called "Hello, world!" and append into it.
          ImGui::ColorEdit3("COLOR", (float *)&colorCube); // Edit 3 floats representing a color
          ImGui::DragFloat3("POSITION", &positionCube.x, 0.001f, -5.f, 5.f);
          ImGui::DragFloat3("SCALE", &scaleCube.x, 0.001f, -5.f, 5.f);
          ImGui::DragFloat3("ROTATION (in rad)", &rotationCube.x, 0.001f, -5.f, 5.f);
          ImGui::End();
        }
        myimgui.Update();
        simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
        myimgui.Render(commandBuffer);
        lveRenderer.endSwapChainRenderPass(commandBuffer);
        lveRenderer.endFrame();
      }
    }

    vkDeviceWaitIdle(lveDevice.device());
  }

  // YOU MAKE THE MODELS HERE
  void FirstApp::loadGameObjects()
  {
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

    cube2.model = lveModel2;

    cube2.transform.translation = {0.f, 0.f, 2.5f};

    // gameObjects.push_back(std::move(cube1));
    gameObjects.push_back(std::move(cube2));
  }

  void FirstApp::addVase(glm::vec3 color, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
  {
    std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, "models/smooth_vase.obj", color);
    auto vase = LveGameObject::createGameObject();

    vase.model = lveModel;
    vase.transform.translation = position;
    vase.transform.rotation = rotation;
    vase.transform.scale = scale;

    gameObjects.push_back(std::move(vase));
  }

  void FirstApp::addCube(glm::vec3 color, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
  {
    std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, "models/cube.obj", color);
    auto vase = LveGameObject::createGameObject();

    vase.model = lveModel;
    vase.transform.translation = position;
    vase.transform.rotation = rotation;
    vase.transform.scale = scale;

    gameObjects.push_back(std::move(vase));
  }

}