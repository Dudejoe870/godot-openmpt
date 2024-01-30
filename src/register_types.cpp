#include "register_types.h"

#include <gdextension_interface.h>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "audio_stream_mpt.h"
#include "openmpt_singleton.h"

using namespace godot;

void initialize_openmpt_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
		return;
	}
	std::cout << "Hello, is it me you're looking for?" << std::endl;

	ClassDB::register_class<AudioStreamMPT>();
	ClassDB::register_class<AudioStreamPlaybackMPT>();
	//Engine::get_singleton()->register_singleton("OpenMPT", OpenMPTSingleton::get_singleton());
}

void uninitialize_openmpt_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
		return;
	}
	//Engine::get_singleton()->unregister_singleton("OpenMPT");
	//memdelete(OpenMPTSingleton::get_singleton());
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT godot_openmpt_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_openmpt_module);
	init_obj.register_terminator(uninitialize_openmpt_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SERVERS);

	return init_obj.init();
}
}
