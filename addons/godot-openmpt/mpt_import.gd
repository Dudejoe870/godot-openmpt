@tool
extends EditorPlugin

var import_plugin: EditorImportPlugin = null

func _enter_tree() -> void:
	import_plugin = preload("mpt_importer.gd").new()
	add_import_plugin(import_plugin)


func _exit_tree() -> void:
	remove_import_plugin(import_plugin)
	import_plugin = null
