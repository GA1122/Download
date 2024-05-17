static int setcos_create_file_44(sc_card_t *card, sc_file_t *file)
{
	const u8 bFileStatus = file->status == SC_FILE_STATUS_CREATION ?
		SETEC_LCSI_CREATE : SETEC_LCSI_ACTIVATED;
	u8 bCommands_always = 0;
	int pins[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	u8 bCommands_pin[sizeof(pins)/sizeof(pins[0])];  
	u8 bCommands_key = 0;
	u8 bNumber = 0;
	u8 bKeyNumber = 0;
	unsigned int bMethod = 0;

	 
	const int df_idx[8] = {   
		SC_AC_OP_DELETE, SC_AC_OP_CREATE, SC_AC_OP_CREATE,
		SC_AC_OP_INVALIDATE, SC_AC_OP_REHABILITATE,
		SC_AC_OP_LOCK, SC_AC_OP_DELETE, -1};
	const int ef_idx[8] = {   
		SC_AC_OP_READ, SC_AC_OP_UPDATE, SC_AC_OP_WRITE,
		SC_AC_OP_INVALIDATE, SC_AC_OP_REHABILITATE,
		-1, SC_AC_OP_ERASE, -1};
	const int efi_idx[8] = {   
		SC_AC_OP_READ, SC_AC_OP_ERASE, SC_AC_OP_UPDATE,
		SC_AC_OP_INVALIDATE, SC_AC_OP_REHABILITATE,
		-1, SC_AC_OP_ERASE, -1};

	 
	sc_file_set_prop_attr(file, &bFileStatus, 1);

	 
	if (file->sec_attr_len == 0) {
		const int* p_idx;
		int	       i;
		int	       len = 0;
		u8         bBuf[64];

		 
		switch (file->type){
		case SC_FILE_TYPE_DF:            
			p_idx = df_idx;
			break;
		case SC_FILE_TYPE_INTERNAL_EF:   
			p_idx = efi_idx;
			break;
		default:                         
			p_idx = ef_idx;
			break;
		}

		 
		memset(bCommands_pin, 0, sizeof(bCommands_pin));
		for (i = 7; i >= 0; i--) {   
			bCommands_always <<= 1;
			bCommands_key <<= 1;

			if (p_idx[i] == -1)   
				continue;

			bMethod = acl_to_byte_44(file->acl[ p_idx[i] ], &bNumber);
			 
			switch(bMethod){
			case SC_AC_NONE:			 
				bCommands_always |= 1;
				break;
			case SC_AC_CHV:				 
				if ((bNumber & 0x7F) == 0 || (bNumber & 0x7F) > 7) {
					sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "SetCOS 4.4 PIN refs can only be 1..7\n");
					return SC_ERROR_INVALID_ARGUMENTS;
				}
				bCommands_pin[setcos_pin_index_44(pins, sizeof(pins), (int) bNumber)] |= 1 << i;
				break;
			case SC_AC_TERM:			 
				bKeyNumber = bNumber;	 
				bCommands_key |= 1;
				break;
			}
		}

		 
		if (bCommands_always) {
			bBuf[len++] = 1;
			bBuf[len++] = bCommands_always;
		}
		 
		for (i = 0; i < (int)sizeof(bCommands_pin) && pins[i] != -1; i++) {
			bBuf[len++] = 2;
			bBuf[len++] = bCommands_pin[i];
			if (SETCOS_IS_EID_APPLET(card))
				bBuf[len++] = pins[i];   
			else
				bBuf[len++] = pins[i] & 0x07;   
		}
		 
		if (bCommands_key) {
			bBuf[len++] = 2 | 0x20;			 
			bBuf[len++] = bCommands_key;
			bBuf[len++] = bKeyNumber;
		}
		 
		if ( (file->type == SC_FILE_TYPE_INTERNAL_EF) &&
		     (acl_to_byte_44(file->acl[SC_AC_OP_CRYPTO], &bNumber) == SC_AC_CHV) ) {
			bBuf[len++] = 0x83;
			bBuf[len++] = 0x01;
			bBuf[len++] = 0x2A;   
			bBuf[len++] = bNumber & 0x07;   
		}

		sc_file_set_sec_attr(file, bBuf, len);
	}

	return iso_ops->create_file(card, file);
}
