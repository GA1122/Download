 _PUBLIC_ size_t strlen_m_ext_handle(struct smb_iconv_handle *ic,
 				    const char *s, charset_t src_charset, charset_t dst_charset)
 {
 	size_t count = 0;
 
 #ifdef DEVELOPER
 	switch (dst_charset) {
 	case CH_DOS:
 	case CH_UNIX:
 		smb_panic("cannot call strlen_m_ext() with a variable dest charset (must be UTF16* or UTF8)");
 	default:
 		break;
 	}
 
 	switch (src_charset) {
 	case CH_UTF16LE:
 	case CH_UTF16BE:
 		smb_panic("cannot call strlen_m_ext() with a UTF16 src charset (must be DOS, UNIX, DISPLAY or UTF8)");
 	default:
 		break;
 	}
 #endif
 	if (!s) {
 		return 0;
 	}
 
 	while (*s && !(((uint8_t)*s) & 0x80)) {
 		s++;
 		count++;
 	}
 
 	if (!*s) {
 		return count;
 	}
  
         while (*s) {
                 size_t c_size;
               codepoint_t c = next_codepoint_handle_ext(ic, s, src_charset, &c_size);
//                codepoint_t c = next_codepoint_handle_ext(ic, s, strnlen(s, 5),
//                                                          src_charset, &c_size);
                 s += c_size;
  
                 switch (dst_charset) {
 		case CH_UTF16BE:
 		case CH_UTF16MUNGED:
 			if (c < 0x10000) {
 				 
 				count += 1;
 			} else {
 				 
 				count += 2;
 			}
 			break;
 		case CH_UTF8:
 			 
 			if (c < 0x80) {
 				count += 1;
 			} else if (c < 0x800) {
 				count += 2;
 			} else if (c < 0x10000) {
 				count += 3;
 			} else {
 				count += 4;
 			}
 			break;
 		default:
 			 
 			count += 1;
 		}
 	}
 
 	return count;
 }