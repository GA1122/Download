static int muscle_card_import_key(sc_card_t *card, sc_cardctl_muscle_key_info_t *info)
{
	 
	switch(info->keyType) {
	case 0x02:  
	case 0x03:  
		return msc_import_key(card,
			info->keyLocation,
			info);
	default:
		return SC_ERROR_NOT_SUPPORTED;
	}
}
