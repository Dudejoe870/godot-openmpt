#include <godot_cpp/core/class_db.hpp>

using namespace godot;

#include <gdextension_interface.h>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "audio_stream_mpt.h"
#include "openmpt_singleton.h"

using namespace godot;

static OpenMPT *open_mpt_ptr = nullptr;

void initialize_openmpt_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<AudioStreamMPT>();
	ClassDB::register_class<AudioStreamPlaybackMPT>();
	
	ClassDB::register_class<OpenMPT>();
	open_mpt_ptr = memnew(OpenMPT);
	Engine::get_singleton()->register_singleton("OpenMPT", OpenMPT::get_singleton());
}

void uninitialize_openmpt_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	Engine::get_singleton()->unregister_singleton("OpenMPT");
	memdelete(open_mpt_ptr);
}

extern "C" {
GDExtensionBool GDE_EXPORT godot_openmpt_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_openmpt_module);
	init_obj.register_terminator(uninitialize_openmpt_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
