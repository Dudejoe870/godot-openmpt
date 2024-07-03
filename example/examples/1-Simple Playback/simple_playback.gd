extends Control

func _ready() -> void:
	%PauseBtn.disabled = true

func _process(delta: float) -> void:
	var stream_player = %AudioStreamPlayer
	if stream_player.has_stream_playback():
		var stream_playback = stream_player.get_stream_playback()
		var current_pattern = stream_playback.get_current_pattern()
		%InfoText.text = "Row: %d/%d Order: %d/%d Pattern: %d/%d" % [
			stream_playback.get_current_row(), 
			stream_player.stream.get_pattern_num_rows(current_pattern),
			stream_playback.get_current_order(), 
			stream_player.stream.get_num_orders(), 
			current_pattern, 
			stream_player.stream.get_num_patterns()]
	else:
		%InfoText.text = ""

func _on_play_btn_pressed() -> void:
	%AudioStreamPlayer.play()
	%PauseBtn.disabled = false
func _on_pause_btn_pressed() -> void:
	%AudioStreamPlayer.stream_paused = !%AudioStreamPlayer.stream_paused
	if %AudioStreamPlayer.stream_paused:
		%PlayBtn.disabled = true
		%StopBtn.disabled = true
		%PauseBtn.text = "Unpause"
	else:
		%PlayBtn.disabled = false
		%StopBtn.disabled = false
		%PauseBtn.text = "Pause"
func _on_stop_btn_pressed() -> void:
	%AudioStreamPlayer.stop()
	%PauseBtn.disabled = true
