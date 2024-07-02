@tool
extends EditorImportPlugin

# There is a max file size limit of 2GB on module files, just for validation reasons (no module should be that big)
const MAX_FILE_SIZE: int = 2147483648

func _get_importer_name() -> String:
	return "dudejoe870.openmptimporter"

func _get_visible_name() -> String:
	return "OpenMPT Importer"

func _get_recognized_extensions() -> PackedStringArray:
	return OpenMPT.get_supported_extensions()

func _get_save_extension() -> String:
	return "res"

func _get_resource_type() -> String:
	return "AudioStreamMPT"

func _get_preset_count() -> int:
	return 0

func _get_import_options(path, preset_index) -> Array[Dictionary]:
	return [{
		   		"name": "force/mono",
				"type": TYPE_BOOL,
				"default_value": false
		   },
		   {
		   		"name": "playback/loop_mode",
				"type": TYPE_INT,
				"property_hint": PROPERTY_HINT_ENUM,
				"hint_string": "Disabled,Enabled",
				"default_value": 0
		   },
		   {
		   		"name": "load/skip_plugins",
		   		"type": TYPE_BOOL,
		   		"default_value": false
		   },
		   {
		   		"name": "load/skip_subsongs_init",
		   		"type": TYPE_BOOL,
		   		"default_value": false
		   }]

func _get_option_visibility(path, option_name, options) -> bool:
	return true

func _get_priority() -> float:
	return 1.0

func _get_import_order() -> int:
	return 0

func _import(source_file, save_path, options, r_platform_variants, r_gen_files) -> Error:
	var file = FileAccess.open(source_file, FileAccess.READ)
	if file == null:
		return FileAccess.get_open_error()
	if file.get_length() >= MAX_FILE_SIZE:
		return ERR_INVALID_DATA
	
	var file_data: PackedByteArray = file.get_buffer(file.get_length())
	if !OpenMPT.can_open_file(file_data):
		return ERR_PARSE_ERROR
	
	var stream = AudioStreamMPT.new()
	stream.skip_plugins = options["load/skip_plugins"]
	stream.skip_subsongs_init = options["load/skip_subsongs_init"]
	stream.data = file_data
	if stream.data.is_empty():
		return stream.get_module_error()

	stream.stereo = !options["force/mono"]
	stream.loop_mode = options["playback/loop_mode"]
	return ResourceSaver.save(stream, "%s.%s" % [save_path, _get_save_extension()])
