static int entersafe_select_aid(sc_card_t *card,
								const sc_path_t *in_path,
								sc_file_t **file_out)
{
	int r = 0;

	if (card->cache.valid 
		&& card->cache.current_path.type == SC_PATH_TYPE_DF_NAME
		&& card->cache.current_path.len == in_path->len
		&& memcmp(card->cache.current_path.value, in_path->value, in_path->len)==0 )
	{
		 if(file_out)
		 {
			  *file_out = sc_file_new();
			  if(!file_out)
				   SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_NORMAL, SC_ERROR_OUT_OF_MEMORY);
		 }
	}
	else
	{
		 r = iso_ops->select_file(card,in_path,file_out);
		 SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");

		  
		 card->cache.current_path.type = SC_PATH_TYPE_DF_NAME;
		 card->cache.current_path.len = in_path->len;
		 memcpy(card->cache.current_path.value,in_path->value,in_path->len);
	}
	if (file_out) {
		 sc_file_t *file = *file_out;
		 assert(file);

		 file->type = SC_FILE_TYPE_DF;
		 file->ef_structure = SC_FILE_EF_UNKNOWN;
		 file->path.len = 0;
		 file->size = 0;
		  
		 memcpy(file->name,in_path->value,in_path->len);
		 file->namelen = in_path->len;
		 file->id = 0x0000;
	}
	SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, r);
}