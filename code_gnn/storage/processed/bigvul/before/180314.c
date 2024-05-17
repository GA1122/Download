 parse_wbxml_tag_defined (proto_tree *tree, tvbuff_t *tvb, guint32 offset,
 			 guint32 str_tbl, guint8 *level, guint8 *codepage_stag, guint8 *codepage_attr,
  			 const wbxml_decoding *map)
  {
  	guint32     tvb_len  = tvb_reported_length (tvb);
	guint32     off      = offset;
// 	guint32     off      = offset, last_off;
  	guint32     len;
  	guint       str_len;
  	guint32     ent;
 	guint32     idx;
 	guint8      peek;
 	guint32     tag_len;                      
 	guint8      tag_save_known      = 0;      
 	guint8      tag_new_known       = 0;      
 	const char *tag_save_literal;             
 	const char *tag_new_literal;              
 	guint8      parsing_tag_content = FALSE;  
  	tag_save_literal = NULL;                  
  
  	DebugLog(("parse_wbxml_tag_defined (level = %u, offset = %u)\n", *level, offset));
// 	last_off = off;
  	while (off < tvb_len) {
  		peek = tvb_get_guint8 (tvb, off);
  		DebugLog(("STAG: (top of while) level = %3u, peek = 0x%02X, off = %u, tvb_len = %u\n", *level, peek, off, tvb_len));
 		if ((peek & 0x3F) < 4) switch (peek) {  
 		case 0x00:  
 			*codepage_stag = tvb_get_guint8 (tvb, off+1);
 			proto_tree_add_text (tree, tvb, off, 2,
 					     "      | Tag   | T -->%3d "
 					     "| SWITCH_PAGE (Tag code page)     "
 					     "|",
 					     *codepage_stag);
 			off += 2;
 			break;
 		case 0x01:  
 			if (tag_save_known) {  
 				proto_tree_add_text (tree, tvb, off, 1,
 						     "  %3d | Tag   | T %3d    "
 						     "| END (Known Tag 0x%02X)            "
 						     "| %s</%s>",
 						     *level, *codepage_stag,
 						     tag_save_known, Indent (*level),
 						     tag_save_literal);  
 			} else {  
 				proto_tree_add_text (tree, tvb, off, 1,
 						     "  %3d | Tag   | T %3d    "
 						     "| END (Literal Tag)               "
 						     "| %s</%s>",
 						     *level, *codepage_stag, Indent (*level),
 						     tag_save_literal ? tag_save_literal : "");
 			}
 			(*level)--;
 			off++;
 			 
 			DebugLog(("STAG: level = %u, Return: len = %u\n", *level, off - offset));
 			return (off - offset);
 		case 0x02:  
 			ent = tvb_get_guintvar (tvb, off+1, &len);
 			proto_tree_add_text (tree, tvb, off, 1+len,
 					     "  %3d | Tag   | T %3d    "
 					     "| ENTITY                          "
 					     "| %s'&#%u;'",
 					     *level, *codepage_stag, Indent (*level), ent);
 			off += 1+len;
 			break;
 		case 0x03:  
 			len = tvb_strsize (tvb, off+1);
 			proto_tree_add_text (tree, tvb, off, 1+len,
 					     "  %3d | Tag   | T %3d    "
 					     "| STR_I (Inline string)           "
 					     "| %s\'%s\'",
 					     *level, *codepage_stag, Indent(*level),
 					     tvb_format_text (tvb, off+1, len-1));
 			off += 1+len;
 			break;
 		case 0x40:  
 		case 0x41:  
 		case 0x42:  
 			 
 			len = tvb_strsize (tvb, off+1);
 			proto_tree_add_text (tree, tvb, off, 1+len,
 					     "  %3d | Tag   | T %3d    "
 					     "| EXT_I_%1x    (Extension Token)    "
 					     "| %s(%s: \'%s\')",
 					     *level, *codepage_stag,
 					     peek & 0x0f, Indent (*level),
 					     map_token (map->global, 0, peek),
 					     tvb_format_text (tvb, off+1, len-1));
 			off += 1+len;
 			break;
 		case 0x43:  
 			proto_tree_add_text (tree, tvb, off, 1,
 					     "  %3d | Tag   | T %3d    "
 					     "| PI (XML Processing Instruction) "
 					     "| %s<?xml",
 					     *level, *codepage_stag, Indent (*level));
 			len = parse_wbxml_attribute_list_defined (tree, tvb, off,
 								  str_tbl, *level, codepage_attr, map);
 			 
 			off += len;
 			if (off >= tvb_len) {
 				DebugLog(("STAG: level = %u, ThrowException: len = %u (short frame)\n", *level, off - offset));
 				 
 				THROW(ReportedBoundsError);
 			}
 			proto_tree_add_text (tree, tvb, off-1, 1,
 					     "  %3d | Tag   | T %3d    "
 					     "| END (PI)                        "
 					     "| %s?>",
 					     *level, *codepage_stag, Indent (*level));
 			break;
 		case 0x80:  
 		case 0x81:  
 		case 0x82:  
 			 
 			idx = tvb_get_guintvar (tvb, off+1, &len);
 			{   char *s;
 				if (map->ext_t[peek & 0x03])
 					s = (map->ext_t[peek & 0x03])(tvb, idx, str_tbl);
 				else
 					s = wmem_strdup_printf(wmem_packet_scope(), "EXT_T_%1x (%s)", peek & 0x03,
 							    map_token (map->global, 0, peek));
 				proto_tree_add_text (tree, tvb, off, 1+len,
 						     "  %3d | Tag   | T %3d    "
 						     "| EXT_T_%1x    (Extension Token)    "
 						     "| %s%s",
 						     *level, *codepage_stag, peek & 0x0f, Indent (*level),
 						     s);
 			}
 			off += 1+len;
 			break;
 		case 0x83:  
 			idx = tvb_get_guintvar (tvb, off+1, &len);
 			str_len = tvb_strsize (tvb, str_tbl+idx);
 			proto_tree_add_text (tree, tvb, off, 1+len,
 					     "  %3d | Tag   | T %3d    "
 					     "| STR_T (Tableref string)         "
 					     "| %s\'%s\'",
 					     *level, *codepage_stag, Indent (*level),
 					     tvb_format_text (tvb, str_tbl+idx, str_len-1));
 			off += 1+len;
 			break;
 		case 0xC0:  
 		case 0xC1:  
 		case 0xC2:  
 			 
 			proto_tree_add_text (tree, tvb, off, 1,
 					     "  %3d | Tag   | T %3d    "
 					     "| EXT_%1x      (Extension Token)    "
 					     "| %s(%s)",
 					     *level, *codepage_stag, peek & 0x0f, Indent (*level),
 					     map_token (map->global, 0, peek));
 			off++;
 			break;
 		case 0xC3:  
 			if (tvb_get_guint8 (tvb, 0)) {  
 				char *str;
 				if (tag_save_known) {  
 					if (map->opaque_binary_tag) {
 						str = map->opaque_binary_tag(tvb, off + 1,
 									     tag_save_known, *codepage_stag, &len);
 					} else {
 						str = default_opaque_binary_tag(tvb, off + 1,
 										tag_save_known, *codepage_stag, &len);
 					}
 				} else {  
 					if (map->opaque_literal_tag) {
 						str = map->opaque_literal_tag(tvb, off + 1,
 									      tag_save_literal, *codepage_stag, &len);
 					} else {
 						str = default_opaque_literal_tag(tvb, off + 1,
 										 tag_save_literal, *codepage_stag, &len);
 					}
 				}
 				proto_tree_add_text (tree, tvb, off, 1 + len,
 						     "  %3d | Tag   | T %3d    "
 						     "| OPAQUE (Opaque data)            "
 						     "| %s%s",
 						     *level, *codepage_stag, Indent (*level), str);
 				off += 1 + len;
 			} else {  
 				proto_tree_add_text (tree, tvb, off, 1,
 						     "  %3d | Tag   | T %3d    "
 						     "| RESERVED_2     (Invalid Token!) "
 						     "| WBXML 1.0 parsing stops here.",
 						     *level, *codepage_stag);
 				 
 				off = tvb_len;
 				DebugLog(("STAG: level = %u, Return: len = %u\n", *level, off - offset));
 				return (off - offset);
 			}
 			break;
 
 			 
 		} else {  
 			 
 
 			 
 			tag_len = 0;
 			if ((peek & 0x3F) == 4) {  
 				DebugLog(("STAG: LITERAL tag (peek = 0x%02X, off = %u) - TableRef follows!\n", peek, off));
 				idx = tvb_get_guintvar (tvb, off+1, &tag_len);
 				str_len = tvb_strsize (tvb, str_tbl+idx);
 				tag_new_literal = (const gchar*)tvb_get_ptr (tvb, str_tbl+idx, str_len);
 				tag_new_known = 0;  
 			} else {  
 				tag_new_known = peek & 0x3F;
 				tag_new_literal = map_token (map->tags, *codepage_stag,
 							     tag_new_known);
 				 
 			}
 
 			 
 			if (peek & 0x40) {  
 				 
 				if (parsing_tag_content) {  
 					DebugLog(("STAG: Tag in Tag - RECURSE! (off = %u)\n", off));
 					 
 					(*level)++;
 					len = parse_wbxml_tag_defined (tree, tvb, off, str_tbl,
 								       level, codepage_stag, codepage_attr, map);
 					off += len;
 				} else {  
 					 
 					if ((peek & 0x3F) == 4) {  
 						tag_save_literal = tag_new_literal;
 						tag_save_known = 0;
 					} else {  
 						tag_save_known = tag_new_known;
 						tag_save_literal = tag_new_literal;
 						 
 					}
 					 
 					if (peek & 0x80) {  
 						if (tag_new_known) {  
 							proto_tree_add_text (tree, tvb, off, 1,
 									     "  %3d | Tag   | T %3d    "
 									     "|   Known Tag 0x%02X           (AC) "
 									     "| %s<%s",
 									     *level, *codepage_stag, tag_new_known,
 									     Indent (*level), tag_new_literal);
 							 
 							off++;
 						} else {  
 							proto_tree_add_text (tree, tvb, off, 1,
 									     "  %3d | Tag   | T %3d    "
 									     "| LITERAL_AC (Literal tag)   (AC) "
 									     "| %s<%s",
 									     *level, *codepage_stag, Indent (*level), tag_new_literal);
 							off += 1 + tag_len;
 						}
 						len = parse_wbxml_attribute_list_defined (tree, tvb,
 											  off, str_tbl, *level, codepage_attr, map);
 						 
 						off += len;
 						if (off >= tvb_len) {
 							DebugLog(("STAG: level = %u, ThrowException: len = %u (short frame)\n",
 								  *level, off - offset));
 							 
 							THROW(ReportedBoundsError);
 						}
 						proto_tree_add_text (tree, tvb, off-1, 1,
 								     "  %3d | Tag   | T %3d    "
 								     "| END (attribute list)            "
 								     "| %s>",
 								     *level, *codepage_stag, Indent (*level));
 					} else {  
 						if (tag_new_known) {  
 							proto_tree_add_text (tree, tvb, off, 1,
 									     "  %3d | Tag   | T %3d    "
 									     "|   Known Tag 0x%02X           (.C) "
 									     "| %s<%s>",
 									     *level, *codepage_stag, tag_new_known,
 									     Indent (*level), tag_new_literal);
 							 
 							off++;
 						} else {  
 							proto_tree_add_text (tree, tvb, off, 1,
 									     "  %3d | Tag   | T %3d    "
 									     "| LITERAL_C  (Literal Tag)   (.C) "
 									     "| %s<%s>",
 									     *level, *codepage_stag, Indent (*level),
 									     tag_new_literal);
 							off += 1 + tag_len;
 						}
 					}
 					 
 					parsing_tag_content = TRUE;
 					DebugLog(("Tag in Tag - No recursion this time! (off = %u)\n", off));
 				}
 			} else {  
 				DebugLog(("<Tag/> in Tag - No recursion! (off = %u)\n", off));
 				(*level)++;
 				if (peek & 0x80) {  
 					if (tag_new_known) {  
 						proto_tree_add_text (tree, tvb, off, 1,
 								     "  %3d | Tag   | T %3d    "
 								     "|   Known Tag 0x%02X           (A.) "
 								     "| %s<%s",
 								     *level, *codepage_stag, tag_new_known,
 								     Indent (*level), tag_new_literal);
 						 
 						off++;
 						len = parse_wbxml_attribute_list_defined (tree, tvb,
 											  off, str_tbl, *level, codepage_attr, map);
 						 
 						off += len;
 						if (off > tvb_len) {
 							DebugLog(("STAG: level = %u, ThrowException: len = %u (short frame)\n", *level, off - offset));
 							 
 							THROW(ReportedBoundsError);
 						}
 						proto_tree_add_text (tree, tvb, off-1, 1,
 								     "  %3d | Tag   | T %3d    "
 								     "| END (Known Tag)                 "
 								     "| %s/>",
 								     *level, *codepage_stag, Indent (*level));
 					} else {  
 						proto_tree_add_text (tree, tvb, off, 1,
 								     "  %3d | Tag   | T %3d    "
 								     "| LITERAL_A  (Literal Tag)   (A.) "
 								     "| %s<%s",
 								     *level, *codepage_stag, Indent (*level), tag_new_literal);
 						off += 1 + tag_len;
 						len = parse_wbxml_attribute_list_defined (tree, tvb,
 											  off, str_tbl, *level, codepage_attr, map);
 						 
 						off += len;
 						if (off >= tvb_len) {
 							DebugLog(("STAG: level = %u, ThrowException: len = %u (short frame)\n", *level, off - offset));
 							 
 							THROW(ReportedBoundsError);
 						}
 						proto_tree_add_text (tree, tvb, off-1, 1,
 								     "  %3d | Tag   | T %3d    "
 								     "| END (Literal Tag)               "
 								     "| %s/>",
 								     *level, *codepage_stag, Indent (*level));
 					}
 				} else {  
 					if (tag_new_known) {  
 						proto_tree_add_text (tree, tvb, off, 1,
 								     "  %3d | Tag   | T %3d    "
 								     "|   Known Tag 0x%02x           (..) "
 								     "| %s<%s />",
 								     *level, *codepage_stag, tag_new_known,
 								     Indent (*level), tag_new_literal);
 						 
 						off++;
 					} else {  
 						proto_tree_add_text (tree, tvb, off, 1,
 								     "  %3d | Tag   | T %3d    "
 								     "| LITERAL    (Literal Tag)   (..) "
 								     "| %s<%s />",
 								     *level, *codepage_stag, Indent (*level),
 								     tag_new_literal);
 						off += 1 + tag_len;
 					}
 				}
 				(*level)--;
  				 
  			}
  		}  
// 		if (off < last_off) {
// 			THROW(ReportedBoundsError);
// 		}
// 		last_off = off;
  	}  
  	DebugLog(("STAG: level = %u, Return: len = %u (end of function body)\n", *level, off - offset));
  	return (off - offset);
 }