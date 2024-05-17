 cleanup_pathname(struct archive_write_disk *a)
// cleanup_pathname_fsobj(char *path, int *error_number, struct archive_string *error_string, int flags)
  {
  	char *dest, *src;
  	char separator = '\0';
  
	dest = src = a->name;
// 	dest = src = path;
  	if (*src == '\0') {
		archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
		    "Invalid empty pathname");
// 		if (error_number) *error_number = ARCHIVE_ERRNO_MISC;
// 		if (error_string)
// 		    archive_string_sprintf(error_string,
// 			    "Invalid empty pathname");
  		return (ARCHIVE_FAILED);
  	}
  
 #if defined(__CYGWIN__)
 	cleanup_pathname_win(a);
  #endif
  	 
  	if (*src == '/') {
		if (a->flags & ARCHIVE_EXTRACT_SECURE_NOABSOLUTEPATHS) {
			archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
			                  "Path is absolute");
// 		if (flags & ARCHIVE_EXTRACT_SECURE_NOABSOLUTEPATHS) {
// 			if (error_number) *error_number = ARCHIVE_ERRNO_MISC;
// 			if (error_string)
// 			    archive_string_sprintf(error_string,
// 				    "Path is absolute");
  			return (ARCHIVE_FAILED);
  		}
  
 		separator = *src++;
 	}
 
 	 
 	for (;;) {
 		 
 		if (src[0] == '\0') {
 			break;
 		} else if (src[0] == '/') {
 			 
 			src++;
 			continue;
 		} else if (src[0] == '.') {
 			if (src[1] == '\0') {
 				 
 				break;
 			} else if (src[1] == '/') {
 				 
 				src += 2;
 				continue;
  			} else if (src[1] == '.') {
  				if (src[2] == '/' || src[2] == '\0') {
  					 
					if (a->flags & ARCHIVE_EXTRACT_SECURE_NODOTDOT) {
						archive_set_error(&a->archive,
						    ARCHIVE_ERRNO_MISC,
						    "Path contains '..'");
// 					if (flags & ARCHIVE_EXTRACT_SECURE_NODOTDOT) {
// 						if (error_number) *error_number = ARCHIVE_ERRNO_MISC;
// 						if (error_string)
// 						    archive_string_sprintf(error_string,
// 							    "Path contains '..'");
  						return (ARCHIVE_FAILED);
  					}
  				}
 				 
 			}
 		}
 
 		 
 		if (separator)
 			*dest++ = '/';
 		while (*src != '\0' && *src != '/') {
 			*dest++ = *src++;
 		}
 
 		if (*src == '\0')
 			break;
 
 		 
 		separator = *src++;
 	}
 	 
	if (dest == a->name) {
// 	if (dest == path) {
  		 
 		if (separator)
 			*dest++ = '/';
 		else
 			*dest++ = '.';
 	}
 	 
 	*dest = '\0';
  	return (ARCHIVE_OK);
  }