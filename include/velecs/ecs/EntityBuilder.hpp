#pragma once

#include "velecs/ecs/Entity.hpp"

#include "velecs/ecs/components/Transform.hpp"

namespace velecs::ecs {

class Scene;

/// @class EntityBuilder
/// @brief A builder class for creating entities with a fluent interface.
///
/// EntityBuilder provides a convenient way to create and configure entities
/// using method chaining. It handles the creation of common components
/// and provides methods to set their properties.
class EntityBuilder {
    friend class Entity;

public:
    // Public Fields

    // Constructors and Destructors

    EntityBuilder(Entity* const entity);

    /// @brief Default constructor
    EntityBuilder() = delete;

    /// @brief Conversion operator to get the built entity.
    /// @return The fully configured entity.
    inline operator Entity*() const {
        return _entity;
    }

    /// @brief Default deconstructor.
    ~EntityBuilder() = default;

    // Public Methods

    /// @brief Sets the name of the entity.
    /// @param name The name to assign to the entity.
    /// @return Reference to this builder for method chaining.
    inline EntityBuilder& WithName(const std::string& name)
    {
        _entity->SetName(name);
        return *this;
    }

    /// @brief Sets the parent of the entity.
    /// @param parent The parent entity to which this entity will be attached.
    /// @return Reference to this builder for method chaining.
    inline EntityBuilder& WithParent(Entity* const parent)
    {
        _transform->TrySetParent(parent);
        return *this;
    }

    /// @brief Sets the position of the entity.
    /// @param pos The position vector to assign.
    /// @return Reference to this builder for method chaining.
    inline EntityBuilder& WithPos(const math::Vec3& pos)
    {
        _transform->SetPos(pos);
        return *this;
    }

    /// @brief Sets the scale of the entity.
    /// @param scale The scale vector to assign.
    /// @return Reference to this builder for method chaining.
    inline EntityBuilder& WithScale(const math::Vec3 scale)
    {
        _transform->SetScale(scale);
        return *this;
    }

    /// @brief Sets the rotation of the entity using a quaternion.
    /// @param rot The rotation quaternion to assign.
    /// @return Reference to this builder for method chaining.
    inline EntityBuilder& WithRot(const math::Quat rot)
    {
        _transform->SetRot(rot);
        return *this;
    }

    /// @brief Sets the rotation of the entity using Euler angles in radians.
    /// @param eulerAngles The rotation angles in radians (x=pitch, y=yaw, z=roll).
    /// @return Reference to this builder for method chaining.
    inline EntityBuilder& WithEulerAngles(const math::Vec3 eulerAngles)
    {
        _transform->SetEulerAnglesRad(eulerAngles);
        return *this;
    }

    /// @brief Sets the rotation of the entity using Euler angles in degrees.
    /// @param eulerAnglesDeg The rotation angles in degrees (x=pitch, y=yaw, z=roll).
    /// @return Reference to this builder for method chaining.
    inline EntityBuilder& WithEulerAnglesDeg(const math::Vec3 eulerAnglesDeg)
    {
        _transform->SetEulerAnglesDeg(eulerAnglesDeg);
        return *this;
    }

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    /// @brief Pointer to entity being constructed.
    Entity* const _entity{nullptr};

    /// @brief Pointer to the entity's transform component.
    Transform* _transform{nullptr};

    // Private Methods
};

} // namespace velecs::ecs
