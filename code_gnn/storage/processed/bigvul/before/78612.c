pgp_strip_path(sc_card_t *card, const sc_path_t *path)
{
	unsigned int start_point = 0;
	 
	if (path->len == 0)
		return 0;

	 
	start_point = (memcmp(path->value, "\x3f\x00", 2) == 0) ? 2 : 0;
	 
	start_point += (memcmp(path->value + start_point, "\x50\x15", 2) == 0) ? 2 : 0;
	return start_point;
}
