#pragma once

#include "lve_model.hpp"

#include <memory>

namespace lve
{

  struct Transform2dComponent
  {
    glm::vec2 translation{};
    glm::vec2 scale{1.f, 1.f};
    float rotation;

    glm::mat2 mat2()
    {
      const float s = glm::sin(rotation);
      const float c = glm::cos(rotation);
      glm::mat2 rotationMat{{c, s}, {-s, c}};

      glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};

      return rotationMat * scaleMat;
    }
  };

  class LveGameObject
  {
  public:
    using id_t = unsigned int;
    std::shared_ptr<LveModel> model{};
    glm::vec3 color{};
    Transform2dComponent transform2d{};

    static LveGameObject createGameObject()
    {
      static id_t currentid = 0;
      return LveGameObject{currentid++};
    }

    LveGameObject(const LveGameObject &) = delete;
    LveGameObject &operator=(const LveGameObject &) = delete;
    LveGameObject(LveGameObject &&) = default;
    LveGameObject &operator=(LveGameObject &&) = default;

    const id_t getId()
    {
      return id;
    }

  private:
    id_t id;
    LveGameObject(id_t objId) : id{objId} {}
  };
} // namespace lve
