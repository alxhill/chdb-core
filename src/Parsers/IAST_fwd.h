#pragma once

#include <boost/container/vector.hpp>
#include <boost/smart_ptr/intrusive_ptr.hpp>

namespace DB
{

class IAST;

void intrusive_ptr_add_ref(const IAST * p) noexcept;
void intrusive_ptr_release(const IAST * p) noexcept;

using ASTPtr = boost::intrusive_ptr<IAST>;
/// Boost vector with smaller stored size to save memory for AST children vectors.
/// (Only meaningful on 64-bit: boost requires stored_size to be strictly narrower
/// than the allocator's size type, which uint32_t is not on 32-bit targets.)
#if __SIZEOF_SIZE_T__ >= 8
using ASTs = boost::container::vector<
    ASTPtr,
    boost::container::new_allocator<ASTPtr>,
    boost::container::vector_options<boost::container::stored_size<uint32_t>>::type>;
#else
using ASTs = boost::container::vector<ASTPtr, boost::container::new_allocator<ASTPtr>>;
#endif

template <typename T, typename ... Args>
constexpr boost::intrusive_ptr<T> make_intrusive(Args && ... args)
{
    return boost::intrusive_ptr<T>(new T(std::forward<Args>(args)...));
}

}
