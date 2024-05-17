static BOOL update_read_play_sound(wStream* s, PLAY_SOUND_UPDATE* play_sound)
{
	if (Stream_GetRemainingLength(s) < 8)
		return FALSE;

	Stream_Read_UINT32(s, play_sound->duration);  
	Stream_Read_UINT32(s, play_sound->frequency);  
	return TRUE;
}
