static gint64 toshiba_seek_next_packet(wtap *wth, int *err, gchar **err_info)
{
	int byte;
	guint level = 0;
	gint64 cur_off;

	while ((byte = file_getc(wth->fh)) != EOF) {
		if (byte == toshiba_rec_magic[level]) {
			level++;
			if (level >= TOSHIBA_REC_MAGIC_SIZE) {
				 
				cur_off = file_tell(wth->fh);
				if (cur_off == -1) {
					 
					*err = file_error(wth->fh, err_info);
					return -1;
				}
				return cur_off + 1;
			}
		} else {
			level = 0;
		}
	}
	 
	*err = file_error(wth->fh, err_info);
	return -1;
}