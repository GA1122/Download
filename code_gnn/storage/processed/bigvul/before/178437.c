 gsicc_open_search(const char* pname, int namelen, gs_memory_t *mem_gc,
                   const char* dirname, int dirlen, stream**strp)
 {
     char *buffer;
     stream* str;
 
      
     if ( dirname != NULL) {
          
         buffer = (char *) gs_alloc_bytes(mem_gc, namelen + dirlen + 1,
                                      "gsicc_open_search");
         if (buffer == NULL)
             return_error(gs_error_VMerror);
         strcpy(buffer, dirname);
         strcat(buffer, pname);
          
         buffer[namelen + dirlen] = '\0';
         str = sfopen(buffer, "r", mem_gc);
         gs_free_object(mem_gc, buffer, "gsicc_open_search");
         if (str != NULL) {
             *strp = str;
 	    return 0;
         }
      }
  
       
    str = sfopen(pname, "r", mem_gc);
    if (str != NULL) {
        *strp = str;
        return 0;
//     if (gs_check_file_permission(mem_gc, pname, namelen, "r") >= 0) {
//         str = sfopen(pname, "r", mem_gc);
//         if (str != NULL) {
//             *strp = str;
//             return 0;
//         }
      }
  
         
                         strlen(DEFAULT_DIR_ICC),"gsicc_open_search");
     if (buffer == NULL)
         return_error(gs_error_VMerror);
     strcpy(buffer, DEFAULT_DIR_ICC);
     strcat(buffer, pname);
      
     buffer[namelen + strlen(DEFAULT_DIR_ICC)] = '\0';
     str = sfopen(buffer, "r", mem_gc);
     gs_free_object(mem_gc, buffer, "gsicc_open_search");
     if (str == NULL) {
         gs_warn1("Could not find %s ",pname);
     }
     *strp = str;
     return 0;
 }