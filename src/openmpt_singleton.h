#ifndef OPENMPT_SINGLETON_H
#define OPENMPT_SINGLETON_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/typed_array.hpp>

using namespace godot;

class OpenMPT;

class OpenMPT : public Object {
	GDCLASS(OpenMPT, Object);
private:
	static OpenMPT *instance;
protected:
	static void _bind_methods();
public:
	static OpenMPT *get_singleton();

	TypedArray<String> get_supported_extensions() const;

	bool can_open_file(const PackedByteArray& p_data) const;

	OpenMPT();
};

#endif // OPENMPT_SINGLETON_H
