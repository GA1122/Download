static int entersafe_select_path(sc_card_t *card,
								const u8 pathbuf[16], const size_t len,
								sc_file_t **file_out)
{
	 u8 n_pathbuf[SC_MAX_PATH_SIZE];
	 const u8 *path=pathbuf;
	 size_t pathlen=len;
	 int bMatch = -1;
	 unsigned int i;
	 int r;

	 if (pathlen%2 != 0 || pathlen > 6 || pathlen <= 0)
		  SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, SC_ERROR_INVALID_ARGUMENTS);

	  
	 if (pathlen == 6 && ( path[0] != 0x3f || path[1] != 0x00 ))
		  SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, SC_ERROR_INVALID_ARGUMENTS);

	  
	 if (path[0] != 0x3f || path[1] != 0x00)
	 {
		  n_pathbuf[0] = 0x3f;
		  n_pathbuf[1] = 0x00;
		  for (i=0; i< pathlen; i++)
			   n_pathbuf[i+2] = pathbuf[i];
		  path = n_pathbuf;
		  pathlen += 2; 
	 }
	
	  
	 if (card->cache.valid 
		 && card->cache.current_path.type == SC_PATH_TYPE_PATH
		 && card->cache.current_path.len >= 2
		 && card->cache.current_path.len <= pathlen )
	 {
		  bMatch = 0;
		  for (i=0; i < card->cache.current_path.len; i+=2)
			   if (card->cache.current_path.value[i] == path[i] 
				   && card->cache.current_path.value[i+1] == path[i+1] )
					bMatch += 2;
	 }

	 if ( card->cache.valid && bMatch > 2 )
	 {
		  if ( pathlen - bMatch == 2 )
		  {
			    
			   return entersafe_select_fid(card, path[bMatch], path[bMatch+1], file_out);
		  }
		  else if ( pathlen - bMatch > 2 )
		  {
			    
			   sc_path_t new_path;
	
			    
			   r = entersafe_select_fid(card, path[bMatch], path[bMatch+1], NULL);
			   SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "SELECT FILE (DF-ID) failed");
	
		   	   memset(&new_path, 0, sizeof(sc_path_t));

			   new_path.type = SC_PATH_TYPE_PATH;
			   new_path.len  = pathlen - bMatch-2;
			   memcpy(new_path.value, &(path[bMatch+2]), new_path.len);
			    
			   return entersafe_select_file(card, &new_path, file_out);
		  }
		  else  
		  {
			    
			   sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL,
				"cache hit\n");
			    
			   if (file_out) {
					sc_file_t *file = sc_file_new();
					if (!file)
						 SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_NORMAL, SC_ERROR_OUT_OF_MEMORY);
					file->id = (path[pathlen-2] << 8) +
						 path[pathlen-1];
					file->path = card->cache.current_path;
					file->type = SC_FILE_TYPE_DF;
					file->ef_structure = SC_FILE_EF_UNKNOWN;
					file->size = 0;
					file->namelen = 0;
					file->magic = SC_FILE_MAGIC;
					*file_out = file;
			   }
			    
			   return SC_SUCCESS;
		  }
	 }
	 else
	 {
		   
		  for ( i=0; i<pathlen-2; i+=2 )
		  {
			   r = entersafe_select_fid(card, path[i], path[i+1], NULL);
			   SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "SELECT FILE (DF-ID) failed");
		  }
		  return entersafe_select_fid(card, path[pathlen-2], path[pathlen-1], file_out);
	 }
}