 static int piv_match_card_continued(sc_card_t *card)
 {
 	int i;
 	int type  = -1;
 	piv_private_data_t *priv = NULL;
 	int saved_type = card->type;
 
 	 
 	card->ops->logout = NULL;
 
 	 
 	 
 	switch (card->type) {
 		case -1:
 		case SC_CARD_TYPE_PIV_II_GENERIC:
 		case SC_CARD_TYPE_PIV_II_HIST:
 		case SC_CARD_TYPE_PIV_II_NEO:
 		case SC_CARD_TYPE_PIV_II_YUBIKEY4:
 		case SC_CARD_TYPE_PIV_II_GI_DE:
 			type = card->type;
 			break;
 		default:
 			return 0;  
 	}
 	if (type == -1) {
 
 		 
 
 		if (card->reader->atr_info.hist_bytes != NULL) {
 			if (card->reader->atr_info.hist_bytes_len == 8 &&
 					!(memcmp(card->reader->atr_info.hist_bytes, "Yubikey4", 8))) {
 				type = SC_CARD_TYPE_PIV_II_YUBIKEY4;
 			}
 			else if (card->reader->atr_info.hist_bytes_len >= 7 &&
 					!(memcmp(card->reader->atr_info.hist_bytes, "Yubikey", 7))) {
 				type = SC_CARD_TYPE_PIV_II_NEO;
 			}
 			 
			else if (card->reader->atr_info.hist_bytes_len >= 4 &&
					!(memcmp(card->reader->atr_info.hist_bytes, "sfte", 4))) {
// 			else if (card->reader->atr_info.hist_bytes_len >= 4
// 					&& !(memcmp(card->reader->atr_info.hist_bytes, "sfte", 4))) {
  				type = SC_CARD_TYPE_PIV_II_GI_DE;
  			}
  
			else if (card->reader->atr_info.hist_bytes[0] == 0x80u) {  
// 			else if (card->reader->atr_info.hist_bytes_len > 0
// 					&& card->reader->atr_info.hist_bytes[0] == 0x80u) {  
  				size_t datalen;
  				const u8 *data = sc_compacttlv_find_tag(card->reader->atr_info.hist_bytes + 1,
  									card->reader->atr_info.hist_bytes_len - 1,
 									0xF0, &datalen);
 
 				if (data != NULL) {
 					int k;
 
 					for (k = 0; piv_aids[k].len_long != 0; k++) {
 						if (datalen == piv_aids[k].len_long
 							&& !memcmp(data, piv_aids[k].value, datalen)) {
 							type = SC_CARD_TYPE_PIV_II_HIST;
 							break;
 						}
 					}
 				}
 			}
 		}
 		if (type == -1)
 			type = SC_CARD_TYPE_PIV_II_GENERIC;
 	}
 
 	 
 
 	priv = calloc(1, sizeof(piv_private_data_t));
 
 	if (!priv)
 		LOG_FUNC_RETURN(card->ctx, SC_ERROR_OUT_OF_MEMORY);
 
 	if (card->type == -1)
 		card->type = type;
 
 	card->drv_data = priv;  
 	priv->aid_file = sc_file_new();
 	priv->selected_obj = -1;
 	priv->pin_preference = 0x80;  
 	priv->logged_in = SC_PIN_STATE_UNKNOWN;
 	priv->tries_left = 10;  
 	priv->pstate = PIV_STATE_MATCH;
 
 	 
 	for (i=0; i < PIV_OBJ_LAST_ENUM -1; i++)
 		if(piv_objects[i].flags & PIV_OBJECT_NOT_PRESENT)
 			priv->obj_cache[i].flags |= PIV_OBJ_CACHE_NOT_PRESENT;
 
 	sc_lock(card);
 
 	 
 	i = piv_find_discovery(card);
 
 	if (i < 0) {
 		 
 		sc_file_t aidfile;
 
 		i = piv_find_aid(card, &aidfile);
 	}
 
 	if (i >= 0) {
 		 
 		int i7e = piv_find_discovery(card);
 
 		if (i7e != 0 && i7e !=  SC_ERROR_FILE_NOT_FOUND) {
 			priv->card_issues |= CI_DISCOVERY_USELESS;
 			priv->obj_cache[PIV_OBJ_DISCOVERY].flags |= PIV_OBJ_CACHE_NOT_PRESENT;
 		}
 	}
 
 
 	if (i < 0) {
 		 
 		sc_unlock(card);
 		piv_finish(card);
 		card->type = saved_type;
 		return 0;
 	}
 
 	 
 	priv->pstate=PIV_STATE_INIT;
 	return 1;  
 }