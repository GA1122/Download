dissect_wbxml_common(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree,
		     const wbxml_decoding *override_content_map)
{
	 
	proto_item           *ti;
	proto_tree           *wbxml_tree;           
	proto_tree           *wbxml_str_tbl_tree;   
	proto_tree           *wbxml_content_tree;   
	guint8                version;
	guint                 offset          = 0;
	guint32               len;
	guint32               charset         = 0;
	guint32               charset_len     = 0;
	guint32               publicid;
	guint32               publicid_index  = 0;
	guint32               publicid_len;
	guint32               str_tbl;
	guint32               str_tbl_len;
	guint32               str_tbl_len_len = 0;
	guint8                level           = 0;  
	const wbxml_decoding *content_map     = NULL;
	gchar                *summary         = NULL;
	guint8                codepage_stag   = 0;
	guint8                codepage_attr   = 0;

	DebugLog(("dissect_wbxml: Dissecting packet %u\n", pinfo->fd->num));
	 
	switch ( version = tvb_get_guint8 (tvb, 0) ) {
	case 0x00:  
		break;

	case 0x01:  
	case 0x02:  
	case 0x03:  
		break;

	default:
		 

		 
		col_append_fstr(pinfo->cinfo, COL_INFO, " (Unknown WBXML version 0x%02x)", version);
		ti = proto_tree_add_item (tree, proto_wbxml, tvb, 0, -1, ENC_NA);
		proto_item_append_text(ti, ", Unknown version 0x%02x", version);
		return;
	}

	 

	 
	publicid = tvb_get_guintvar(tvb, 1, &publicid_len);
	if (! publicid) {
		 
		publicid_index = tvb_get_guintvar (tvb, 1+publicid_len, &len);
		publicid_len += len;
	}
	offset = 1 + publicid_len;

	 
	switch ( version ) {
	case 0x00:  
		 
		break;

	case 0x01:  
	case 0x02:  
	case 0x03:  
		 
		charset = tvb_get_guintvar (tvb, offset, &charset_len);
		offset += charset_len;
		break;

	default:  
		DISSECTOR_ASSERT_NOT_REACHED();
		break;
	}

	 
	tvb_get_guintvar (tvb, offset, &str_tbl_len_len);
	str_tbl = offset + str_tbl_len_len;  

	 
	if ( publicid ) {
		summary = wmem_strdup_printf(wmem_packet_scope(), "%s, Public ID: \"%s\"",
					  val_to_str_ext (version, &vals_wbxml_versions_ext, "(unknown 0x%x)"),
					  val_to_str_ext (publicid, &vals_wbxml_public_ids_ext, "(unknown 0x%x)"));
	} else {
		 
		len = tvb_strsize (tvb, str_tbl + publicid_index);
		summary = wmem_strdup_printf(wmem_packet_scope(), "%s, Public ID: \"%s\"",
					  val_to_str_ext (version, &vals_wbxml_versions_ext, "(unknown 0x%x)"),
					  tvb_format_text (tvb, str_tbl + publicid_index, len - 1));
	}

	 
	col_append_fstr(pinfo->cinfo, COL_INFO, " (WBXML %s)", summary);

	 
	ti = proto_tree_add_item (tree, proto_wbxml, tvb, 0, -1, ENC_NA);
	proto_item_append_text(ti, ", Version: %s", summary);

	 
	if ( tree ) {
		wbxml_tree = proto_item_add_subtree(ti, ett_wbxml);

		 
		proto_tree_add_uint (wbxml_tree, hf_wbxml_version,
				     tvb, 0, 1, version);

		 
		if (publicid) {  
			proto_tree_add_uint(wbxml_tree, hf_wbxml_public_id_known,
					    tvb, 1, publicid_len, publicid);
		} else {  
			proto_tree_add_item (wbxml_tree, hf_wbxml_public_id_literal,
					     tvb, 1, publicid_len, ENC_ASCII|ENC_NA);
		}
		offset = 1 + publicid_len;

		if ( version ) {  
			proto_tree_add_uint (wbxml_tree, hf_wbxml_charset,
					     tvb, 1 + publicid_len, charset_len, charset);
			offset += charset_len;
		}

		str_tbl_len = tvb_get_guintvar (tvb, offset, &len);
		str_tbl = offset + len;  

		 
		ti = proto_tree_add_text(wbxml_tree,
					 tvb, offset, len + str_tbl_len, "String table: %u bytes",
					 str_tbl_len);

		if (wbxml_tree && str_tbl_len) {  
			wbxml_str_tbl_tree = proto_item_add_subtree (ti,
								     ett_wbxml_str_tbl);
			show_wbxml_string_table (wbxml_str_tbl_tree, tvb,
						 str_tbl, str_tbl_len);
		}

		 
		offset += len + str_tbl_len;

		 
		if (disable_wbxml_token_parsing) {
			proto_tree_add_text (wbxml_tree, tvb, offset, -1,
						  "Data representation not shown "
						  "(edit WBXML preferences to show)");
			return;
		}  
		ti = proto_tree_add_text (wbxml_tree, tvb, offset, -1,
					  "Data representation");
		wbxml_content_tree = proto_item_add_subtree (ti, ett_wbxml_content);

		 
		if (wbxml_tree) {  
			if (override_content_map != NULL) {
				content_map = override_content_map;
				proto_item_append_text(ti,
						       " is based on: %s",
						       content_map->name);
			} else {
				 
				content_map = get_wbxml_decoding_from_public_id (publicid);
				if (! content_map) {
					content_map = get_wbxml_decoding_from_content_type(
											   pinfo->match_string, tvb, offset);
					if (! content_map) {
						proto_tree_add_text (wbxml_content_tree,
								     tvb, offset, -1,
								     "[Rendering of this content type"
								     " not (yet) supported]");
					} else {
						proto_item_append_text(ti,
								       " is based on Content-Type: %s "
								       "(chosen decoding: %s)",
								       pinfo->match_string, content_map->name);
					}
				}
			}
			if (content_map && skip_wbxml_token_mapping) {
				proto_tree_add_text (wbxml_content_tree,
						     tvb, offset, -1,
						     "[Rendering of this content type"
						     " has been disabled "
						     "(edit WBXML preferences to enable)]");
				content_map = NULL;
			}
			proto_tree_add_text (wbxml_content_tree, tvb,
					     offset, -1,
					     "Level | State | Codepage "
					     "| WBXML Token Description         "
					     "| Rendering");
			if (content_map) {
				len = parse_wbxml_tag_defined (wbxml_content_tree,
							       tvb, offset, str_tbl, &level, &codepage_stag,
							       &codepage_attr, content_map);
			} else {
				 
				len = parse_wbxml_tag (wbxml_content_tree, tvb, offset,
						       str_tbl, &level, &codepage_stag, &codepage_attr);
			}
		}
		return;
	}
}
