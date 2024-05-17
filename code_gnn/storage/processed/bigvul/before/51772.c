static gboolean cosine_read(wtap *wth, int *err, gchar **err_info,
    gint64 *data_offset)
{
	gint64	offset;
	char	line[COSINE_LINE_LENGTH];

	 
	offset = cosine_seek_next_packet(wth, err, err_info, line);
	if (offset < 0)
		return FALSE;
	*data_offset = offset;

	 
	return parse_cosine_packet(wth->fh, &wth->phdr, wth->frame_buffer,
	    line, err, err_info);
}