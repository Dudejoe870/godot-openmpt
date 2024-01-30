#include "openmpt_singleton.h"

#include <godot_cpp/core/class_db.hpp>

#include <libopenmpt/libopenmpt.hpp>

OpenMPT* OpenMPT::instance = nullptr;

OpenMPT*OpenMPT::get_singleton() {
	if (!instance) instance = memnew(OpenMPT);
	return instance;
}

TypedArray<String> OpenMPT::get_supported_extensions() const {
	TypedArray<String> result;
	std::vector<std::string> extensions = openmpt::get_supported_extensions();
	for (std::string ext : extensions)
		result.push_back(String(ext.c_str()));
	return result;
}

void OpenMPT::_bind_methods() {
	ClassDB::bind_static_method("OpenMPT", D_METHOD("get_singleton"), &OpenMPT::get_singleton);

	ClassDB::bind_method(D_METHOD("get_supported_extensions"), &OpenMPT::get_supported_extensions);
}

OpenMPT::OpenMPT() {}
