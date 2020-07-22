#ifndef NETHELPER_NONCOPYABLE_H
#define NETHELPER_NONCOPYABLE_H

namespace nethelper {

class Noncopyable {
    public:
	Noncopyable(const Noncopyable&) = delete;
	void operator=(const Noncopyable&) = delete;

    protected:
	Noncopyable()  = default;
	~Noncopyable() = default;
};

}  // namespace nethelper

#endif
