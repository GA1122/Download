static int piv_get_key(sc_card_t *card, unsigned int alg_id, u8 **key, size_t *len)
{

	int r;
	size_t fsize;
	FILE *f = NULL;
	char * keyfilename = NULL;
	size_t expected_keylen;
	size_t keylen;
	u8 * keybuf = NULL;
	u8 * tkey = NULL;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	keyfilename = (char *)getenv("PIV_EXT_AUTH_KEY");

	if (keyfilename == NULL) {
		sc_log(card->ctx,
			"Unable to get PIV_EXT_AUTH_KEY=(null) for general_external_authenticate");
		r = SC_ERROR_FILE_NOT_FOUND;
		goto err;
	}

	r = get_keylen(alg_id, &expected_keylen);
	if(r) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Invalid cipher selector, none found for:  %02x", alg_id);
		r = SC_ERROR_INVALID_ARGUMENTS;
		goto err;
	}

	f = fopen(keyfilename, "rb");
	if (!f) {
		sc_log(card->ctx, " Unable to load key from file\n");
		r = SC_ERROR_FILE_NOT_FOUND;
		goto err;
	}

	if (0 > fseek(f, 0L, SEEK_END))
		r = SC_ERROR_INTERNAL;
	fsize = ftell(f);
	if (0 > (long) fsize)
		r = SC_ERROR_INTERNAL;
	if (0 > fseek(f, 0L, SEEK_SET))
		r = SC_ERROR_INTERNAL;
	if(r) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Could not read %s\n", keyfilename);
		goto err;
	}

	keybuf = malloc(fsize+1);  
	if (!keybuf) {
		sc_log(card->ctx, " Unable to allocate key memory");
		r = SC_ERROR_OUT_OF_MEMORY;
		goto err;
	}
	keybuf[fsize] = 0x00;     

	if (fread(keybuf, 1, fsize, f) != fsize) {
		sc_log(card->ctx, " Unable to read key\n");
		r = SC_ERROR_WRONG_LENGTH;
		goto err;
	}

	tkey = malloc(expected_keylen);
	if (!tkey) {
	    sc_log(card->ctx, " Unable to allocate key memory");
	    r = SC_ERROR_OUT_OF_MEMORY;
	    goto err;
	}

	if (fsize == expected_keylen) {  
		memcpy(tkey, keybuf, expected_keylen);
	} else {
		 
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Treating key as hex-encoded!\n");
		sc_right_trim(keybuf, fsize);
		keylen = expected_keylen;
		r = sc_hex_to_bin((char *)keybuf, tkey, &keylen);
		if (keylen !=expected_keylen || r != 0 ) {
			sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Error formatting key\n");
			if (r == 0)
				r = SC_ERROR_INCOMPATIBLE_KEY;
			goto err;
		}
	}
	*key = tkey;
	tkey = NULL;
	*len = expected_keylen;
	r = SC_SUCCESS;

err:
	if (f)
		fclose(f);
	if (keybuf) {
		free(keybuf);
	}
	if (tkey) {
		free(tkey);
	}

	LOG_FUNC_RETURN(card->ctx, r);
	return r;
}