#pragma once

/**
 * \brief Wraps a return statement in a block for inline usage.
 * \param v the returned value
 */
#define INLINE_RETURN(v) { return v; }

/**
 * \brief Wraps a return statement in a block for inline usage, and adds noexcept for compile time optimization.
 * \param v the returned value
 */
#define INLINE_NOEXCEPT(v) noexcept { return v; }

/**
 * \brief Wraps a return statement in a block for inline usage in a static constant method.
 * \param type the return type of the method
 * \param method the name of the method
 * \param code the returned value
 */
#define STATIC_CONST_INLINE(type, method, code) \
static constexpr type method() { return code; }

/**
 * \brief Wraps a return statement in a block for inline usage in a static constant method, and adds noexcept for compile time optimization.
 * \param type the return type of the method
 * \param method the name of the method
 * \param code the returned value
 */
#define STATIC_CONST_INLINE_NOEXCEPT(type, method, code) \
static constexpr type method() noexcept { return code; }
