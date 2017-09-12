#pragma once
#include <functional>

namespace esgui {

enum class action { down, up, move, cancel };

template <class Signature>
using event = std::function<Signature>;

/*template <class Signature>
class event
{
public:
	template <class T>
	void set(T clb) {
		m_clb = clb;
	}
	template <class... T>
	void operator() (T&&... args) {
		m_clb(std::forward<T>(args)...);
	}

private:
	std::function<Signature> m_clb;
};*/

}