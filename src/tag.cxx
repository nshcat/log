#include <tag.hxx>

namespace lg
{
	auto tag(::std::string p_str)
		-> internal::tag_t
	{
		return { ::std::move(p_str) };
	}
}
