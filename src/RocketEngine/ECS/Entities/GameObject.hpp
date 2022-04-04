#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "RocketEngine/ECS/Components/AComponent.hh"

namespace rocketengine::ecs
{
    template <typename T>
    concept ComponentType = std::is_base_of_v<AComponent, T>;

    class GameObject
    {
    private:
        std::string name;
        std::vector<std::unique_ptr<AComponent>> components;

    public:
        GameObject(std::string_view _name) noexcept;
        GameObject(GameObject const& rhs) noexcept = default;
        GameObject(GameObject&& rhs) noexcept = default;

        ~GameObject() noexcept = default;

        GameObject& operator=(GameObject const& rhs) noexcept = default;
        GameObject& operator=(GameObject&& rhs) noexcept = default;

        template<typename ComponentType, typename... Args>
        void addComponent(Args&& args)
        {
            this->components.emplace_back(std::make_unique<ComponentType>(*this, std::forward<Args>(args)...));
        }

        template<typename ComponentType>
        void removeComponent()
        {
            std::remove_if(
                this->components.begin(),
                this->components.end(),
                [](std::unique_ptr<AComponent> const& element)
                {
                    return dynamic_cast<ComponentType *>(*element);
                }
            );
        }
    };
} // namespace rocketengine::ecs