#include "openmpt_singleton.h"

#include <godot_cpp/core/class_db.hpp>

#include <libopenmpt/libopenmpt.hpp>

OpenMPT *OpenMPT::instance = nullptr;

OpenMPT *OpenMPT::get_singleton() {
	return instance;
}

TypedArray<String> OpenMPT::get_supported_extensions() const {
	TypedArray<String> result;
	std::vector<std::string> extensions = openmpt::get_supported_extensions();
	for (std::string ext : extensions)
		result.push_back(String(ext.c_str()));
	return result;
}

bool OpenMPT::can_open_file(const PackedByteArray& p_data) const {
	size_t recommended_size = openmpt::probe_file_header_get_recommended_size();

	if (p_data.size() < recommended_size)
		return openmpt::probe_file_header(openmpt::probe_file_header_flags_default2, p_data.ptr(), p_data.size(), p_data.size())
			== openmpt::probe_file_header_result_success;

	return openmpt::probe_file_header(openmpt::probe_file_header_flags_default2, p_data.ptr(), recommended_size) 
		== openmpt::probe_file_header_result_success;
}

void OpenMPT::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_supported_extensions"), &OpenMPT::get_supported_extensions);

	ClassDB::bind_method(D_METHOD("can_open_file", "data"), &OpenMPT::can_open_file);
}

OpenMPT::OpenMPT() {
	instance = this;
}
