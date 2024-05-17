map_token (const value_valuestring *token_map, guint8 codepage, guint8 token) {
	const value_string *vs;
	const char         *s;

	if (token_map) {  
		if ((vs = val_to_valstr (codepage, token_map))) {
			 
			s = try_val_to_str (token, vs);
			if (s) {  
				DebugLog(("map_token(codepage = %u, token = %u: [%s]\n", codepage, token, s));
				return s;
			}
			 
			DebugLog(("map_token(codepage = %u, token = %u: "
				  wbxml_UNDEFINED_TOKEN "\n", codepage, token));
			return wbxml_UNDEFINED_TOKEN;
		}
		 
		DebugLog(("map_token(codepage = %u, token = %u: "
			  wbxml_UNDEFINED_TOKEN_CODE_PAGE "\n", codepage, token));
		return wbxml_UNDEFINED_TOKEN_CODE_PAGE;
	}
	 
	DebugLog(("map_token(codepage = %u, token = %u: "
		  wbxml_UNDEFINED_TOKEN_MAP "\n", codepage, token));
	return wbxml_UNDEFINED_TOKEN_MAP;
}
