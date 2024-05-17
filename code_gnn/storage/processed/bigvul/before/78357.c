static int entersafe_select_file(sc_card_t *card,
								 const sc_path_t *in_path,
								 sc_file_t **file_out)
{
	 int r;
	 char pbuf[SC_MAX_PATH_STRING_SIZE];
	 assert(card);
	 assert(in_path);
	 SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);


	  r = sc_path_print(pbuf, sizeof(pbuf), &card->cache.current_path);
	  if (r != SC_SUCCESS)
		 pbuf[0] = '\0';

	  sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL,
		   "current path (%s, %s): %s (len: %"SC_FORMAT_LEN_SIZE_T"u)\n",
		   card->cache.current_path.type == SC_PATH_TYPE_DF_NAME ?
		   "aid" : "path",
		   card->cache.valid ? "valid" : "invalid", pbuf,
		   card->cache.current_path.len);

	 switch(in_path->type)
	 {
	 case SC_PATH_TYPE_FILE_ID:
		  if (in_path->len != 2)
			   SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE,SC_ERROR_INVALID_ARGUMENTS);
		  return entersafe_select_fid(card,in_path->value[0],in_path->value[1], file_out);
	 case SC_PATH_TYPE_DF_NAME:
		  return entersafe_select_aid(card,in_path,file_out);
	 case SC_PATH_TYPE_PATH:
		  return entersafe_select_path(card,in_path->value,in_path->len,file_out);
	 default:
		  SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, SC_ERROR_INVALID_ARGUMENTS);
	 }
}