  cosine_seek_read(wtap *wth, gint64 seek_off, struct wtap_pkthdr *phdr,
  	Buffer *buf, int *err, gchar **err_info)
  {
	int	pkt_len;
  	char	line[COSINE_LINE_LENGTH];
  
  	if (file_seek(wth->random_fh, seek_off, SEEK_SET, err) == -1)
 		return FALSE;
 
 	if (file_gets(line, COSINE_LINE_LENGTH, wth->random_fh) == NULL) {
 		*err = file_error(wth->random_fh, err_info);
 		if (*err == 0) {
 			*err = WTAP_ERR_SHORT_READ;
 		}
  		return FALSE;
  	}
  
 	 
	pkt_len = parse_cosine_rec_hdr(phdr, line, err, err_info);
	if (pkt_len == -1)
		return FALSE;
	 
	return parse_cosine_hex_dump(wth->random_fh, phdr, pkt_len, buf, err,
// 	return parse_cosine_packet(wth->random_fh, phdr, buf, line, err,
  	    err_info);
  }