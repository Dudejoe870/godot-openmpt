#ifndef OPENMPT_SINGLETON_H
#define OPENMPT_SINGLETON_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/dictionary.hpp>

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
	bool is_extension_supported(String extension) const;

	Dictionary get_library_version() const;
	Dictionary get_core_version() const;

	bool can_open_file(const PackedByteArray& p_data) const;

	OpenMPT();
};

#endif // OPENMPT_SINGLETON_H
