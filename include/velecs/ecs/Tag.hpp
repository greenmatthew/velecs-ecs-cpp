/// @file    Tag.hpp
/// @author  Matthew Green
/// @date    2025-05-16 13:02:55
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

namespace velecs::ecs {

/// @class Tag
/// @brief Base class for empty component types used as entity markers/flags.
///
/// A Tag is a special kind of component that contains no data and serves only to
/// categorize or mark entities. EnTT optimizes empty component types by not actually
/// instantiating them, which saves memory but means references to tag instances
/// cannot be obtained.
///
/// Tags are useful for:
/// - Filtering entities in queries (views/groups)
/// - Marking entities for special processing
/// - Categorizing entities without additional data overhead
///
/// Usage:
/// 1. Create specific tag types by inheriting from this class
/// 2. Use consistent naming with a "Tag" suffix (e.g., DestroyTag, PlayerTag)
/// 3. Assign tags to entities with registry.emplace<MyTag>(entity)
/// 4. Query entities with tags using registry.view<MyTag>() or registry.all_of<MyTag>(entity)
///
/// Note: Due to EnTT's empty type optimization, you cannot get references to tag
/// instances, but you can check for their existence and iterate entities that have them.
class Tag {
public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Default constructor.
    Tag() = default;

    /// @brief Default deconstructor.
    ~Tag() = default;

    // Public Methods

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    // Private Methods
};

} // namespace velecs::ecs
