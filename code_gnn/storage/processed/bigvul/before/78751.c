static int muscle_check_sw(sc_card_t * card, unsigned int sw1, unsigned int sw2) {
	if(sw1 == 0x9C) {
		switch(sw2) {
			case 0x01:  
				return SC_ERROR_NOT_ENOUGH_MEMORY;
			case 0x02:  
				return SC_ERROR_PIN_CODE_INCORRECT;
			case 0x03:  
				return SC_ERROR_NOT_ALLOWED;
			case 0x05:  
				return SC_ERROR_NO_CARD_SUPPORT;
			case 0x06:  
				return SC_ERROR_SECURITY_STATUS_NOT_SATISFIED;
			case 0x07:  
				return SC_ERROR_FILE_NOT_FOUND;
			case 0x08:  
				return SC_ERROR_FILE_ALREADY_EXISTS;
			case 0x09:  
				return SC_ERROR_INCORRECT_PARAMETERS;
			case 0x0B:  
				return SC_ERROR_CARD_CMD_FAILED;
			case 0x0C:  
				return SC_ERROR_AUTH_METHOD_BLOCKED;
			case 0x0F:  
			case 0x10:  
			case 0x11:  
				return SC_ERROR_INCORRECT_PARAMETERS;
		}
	}
	return iso_ops->check_sw(card, sw1, sw2);
}
