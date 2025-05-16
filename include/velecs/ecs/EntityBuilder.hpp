/// @file    EntityBuilder.hpp
/// @author  Matthew Green
/// @date    2025-05-16 11:43:43
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include "velecs/ecs/Entity.hpp"

#include "velecs/ecs/components/Name.hpp"
#include "velecs/ecs/components/Relationship.hpp"
#include "velecs/ecs/components/Transform.hpp"

namespace velecs::ecs {

/// @class EntityBuilder
/// @brief A builder class for creating entities with a fluent interface.
///
/// EntityBuilder provides a convenient way to create and configure entities
/// using method chaining. It handles the creation of common components
/// and provides methods to set their properties.
class EntityBuilder {
    friend class Entity;

public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Constructor that creates a new entity with default components.
    EntityBuilder();

    /// @brief Conversion operator to get the built entity.
    /// @return The fully configured entity.
    inline operator Entity() const {
        return entity;
    }

    /// @brief Default deconstructor.
    ~EntityBuilder() = default;

    // Public Methods

    /// @brief Sets the name of the entity.
    /// @param name The name to assign to the entity.
    /// @return Reference to this builder for method chaining.
    inline EntityBuilder& WithName(std::string name)
    {
        this->name.SetName(name);
        return *this;
    }

    /// @brief Sets the parent of the entity.
    /// @param parent The parent entity to which this entity will be attached.
    /// @return Reference to this builder for method chaining.
    inline EntityBuilder& WithParent(const Entity parent)
    {
        relationship.SetParent(parent);
        return *this;
    }

    /// @brief Sets the position of the entity.
    /// @param pos The position vector to assign.
    /// @return Reference to this builder for method chaining.
    inline EntityBuilder& WithPos(const math::Vec3& pos)
    {
        transform.SetPos(pos);
        return *this;
    }

    /// @brief Sets the scale of the entity.
    /// @param scale The scale vector to assign.
    /// @return Reference to this builder for method chaining.
    inline EntityBuilder& WithScale(const math::Vec3 scale)
    {
        transform.SetScale(scale);
        return *this;
    }

    /// @brief Sets the rotation of the entity using a quaternion.
    /// @param rot The rotation quaternion to assign.
    /// @return Reference to this builder for method chaining.
    inline EntityBuilder& WithRot(const math::Quat rot)
    {
        transform.SetRot(rot);
        return *this;
    }

    /// @brief Sets the rotation of the entity using Euler angles in radians.
    /// @param eulerAngles The rotation angles in radians (x=pitch, y=yaw, z=roll).
    /// @return Reference to this builder for method chaining.
    inline EntityBuilder& WithEulerAngles(const math::Vec3 eulerAngles)
    {
        transform.SetEulerAngles(eulerAngles);
        return *this;
    }

    /// @brief Sets the rotation of the entity using Euler angles in degrees.
    /// @param eulerAnglesDeg The rotation angles in degrees (x=pitch, y=yaw, z=roll).
    /// @return Reference to this builder for method chaining.
    inline EntityBuilder& WithEulerAnglesDeg(const math::Vec3 eulerAnglesDeg)
    {
        transform.SetEulerAnglesDeg(eulerAnglesDeg);
        return *this;
    }

    /// @brief Adds a component of type T to the entity.
    /// @tparam T The type of component to add.
    /// @return Reference to this builder for method chaining.
    template<typename T, typename = IsComponent<T>>
    inline EntityBuilder& WithComponent()
    {
        entity.AddComponent<T>();
        return *this;
    }

    /// @brief Adds a component of type T to the entity with constructor arguments.
    /// @tparam T The type of component to add.
    /// @tparam Args The types of the constructor arguments.
    /// @param args The constructor arguments.
    /// @return Reference to this builder for method chaining.
    template<typename T, typename = IsComponent<T>, typename... Args>
    inline EntityBuilder& WithComponent(Args &&...args)
    {
        entity.AddComponent<T>(std::forward<Args>(args)...);
        return *this;
    }

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    /// @brief The entity being constructed.
    Entity entity;

    /// @brief Reference to the entity's name component.
    Name& name;
    
    /// @brief Reference to the entity's relationship component.
    Relationship& relationship;
    
    /// @brief Reference to the entity's transform component.
    Transform& transform;

    // Private Methods
};

} // namespace velecs::ecs
