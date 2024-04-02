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
		result.push_back(ext.c_str());
	return result;
}

bool OpenMPT::is_extension_supported(String extension) const {
	return openmpt::is_extension_supported2(extension.ascii().get_data());
}

Dictionary OpenMPT::get_library_version() const {
	Dictionary result;
	uint32_t version = openmpt::get_library_version();
	result["major"] = version >> 24;
	result["minor"] = (version >> 16) & 0xFFFF;
	result["patch"] = version & 0xFF;
	return result;
}

Dictionary OpenMPT::get_core_version() const {
	Dictionary result;
	uint32_t version = openmpt::get_core_version();
	result["majormajor"] = version >> 24;
	result["major"] = (version >> 16) & 0xFF;
	result["minor"] = (version >> 8) & 0xFF;
	result["minorminor"] = version & 0xFF;
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
	ClassDB::bind_method(D_METHOD("is_extension_supported", "extension"), &OpenMPT::is_extension_supported);

	ClassDB::bind_method(D_METHOD("get_library_version"), &OpenMPT::get_library_version);
	ClassDB::bind_method(D_METHOD("get_core_version"), &OpenMPT::get_core_version);

	ClassDB::bind_method(D_METHOD("can_open_file", "data"), &OpenMPT::can_open_file);
}

OpenMPT::OpenMPT() {
	instance = this;
}
