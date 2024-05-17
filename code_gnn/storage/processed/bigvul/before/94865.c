static gboolean netscreen_read(wtap *wth, int *err, gchar **err_info,
    gint64 *data_offset)
{
	gint64		offset;
	char		line[NETSCREEN_LINE_LENGTH];

	 
	offset = netscreen_seek_next_packet(wth, err, err_info, line);
	if (offset < 0)
		return FALSE;

	 
	if (!parse_netscreen_packet(wth->fh, &wth->phdr,
	    wth->frame_buffer, line, err, err_info))
		return FALSE;

	 
	if (wth->file_encap == WTAP_ENCAP_UNKNOWN)
		wth->file_encap = wth->phdr.pkt_encap;
	else {
		if (wth->file_encap != wth->phdr.pkt_encap)
			wth->file_encap = WTAP_ENCAP_PER_PACKET;
	}

	*data_offset = offset;
	return TRUE;
}
