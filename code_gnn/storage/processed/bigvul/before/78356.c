static int entersafe_select_fid(sc_card_t *card,
								unsigned int id_hi, unsigned int id_lo,
								sc_file_t **file_out)
{
	int r;
	sc_file_t *file = NULL;
	sc_path_t path;

	memset(&path, 0, sizeof(sc_path_t));

	path.type=SC_PATH_TYPE_FILE_ID;
	path.value[0]=id_hi;
	path.value[1]=id_lo;
	path.len=2;

	r = iso_ops->select_file(card,&path,&file);
	if (r < 0)
		sc_file_free(file);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");

	 
	if (file->type == SC_FILE_TYPE_DF) {
		 card->cache.current_path.type = SC_PATH_TYPE_PATH;
		 card->cache.current_path.value[0] = 0x3f;
		 card->cache.current_path.value[1] = 0x00;
		 if (id_hi == 0x3f && id_lo == 0x00){
			  card->cache.current_path.len = 2;
		 } else {
			  card->cache.current_path.len = 4;
			  card->cache.current_path.value[2] = id_hi;
			  card->cache.current_path.value[3] = id_lo;
		 }
	}
	
	if (file_out)
		*file_out = file;
	else
		sc_file_free(file);

	SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, SC_SUCCESS);
}