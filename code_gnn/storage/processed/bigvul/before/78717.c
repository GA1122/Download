static int itacns_init(sc_pkcs15_card_t *p15card)
{
	int r;
	sc_path_t path;
	int certificate_count = 0;
	int found_certs;
	int card_is_cie_v1, cns0_secenv;

	SC_FUNC_CALLED(p15card->card->ctx, 1);

	set_string(&p15card->tokeninfo->label, p15card->card->name);
	if(p15card->card->drv_data) {
		unsigned int mask_code, ic_code;
		char buffer[256];
		itacns_drv_data_t *data =
			(itacns_drv_data_t*) p15card->card->drv_data;
		mask_code = data->mask_manufacturer_code;
		if (mask_code >= sizeof(itacns_mask_manufacturers)
			/sizeof(itacns_mask_manufacturers[0]))
			mask_code = 0;
		ic_code = data->ic_manufacturer_code;
		if (ic_code >= sizeof(iso7816_ic_manufacturers)
			/sizeof(iso7816_ic_manufacturers[0]))
			ic_code = 0;
		snprintf(buffer, sizeof(buffer), "IC: %s; mask: %s",
			iso7816_ic_manufacturers[ic_code],
			itacns_mask_manufacturers[mask_code]);
		set_string(&p15card->tokeninfo->manufacturer_id, buffer);
	}

	 
	{
		u8 serial[17];
		int bytes;
		sc_format_path(path_serial, &path);
		bytes = loadFile(p15card, &path, serial, 16);
		if (bytes < 0) return bytes;
		if (bytes > 16) return -1;
		serial[bytes] = '\0';
		set_string(&p15card->tokeninfo->serial_number, (char*)serial);
	}

	 
	card_is_cie_v1 =
		   (p15card->card->type == SC_CARD_TYPE_ITACNS_CIE_V1)
		|| (p15card->card->type == SC_CARD_TYPE_CARDOS_CIE_V1);
	cns0_secenv = (card_is_cie_v1 ? 0x31 : 0x01);

	 
	if (card_is_cie_v1) {
		int i;
		for (i = 0; i < p15card->card->algorithm_count; i++) {
			sc_algorithm_info_t *info =
				&p15card->card->algorithms[i];

			if (info->algorithm != SC_ALGORITHM_RSA)
				continue;
			info->flags &= ~(SC_ALGORITHM_RSA_RAW
				| SC_ALGORITHM_RSA_HASH_NONE);
			info->flags |= (SC_ALGORITHM_RSA_PAD_PKCS1
				| SC_ALGORITHM_RSA_HASHES);
		}
	}

	 
	r = itacns_add_data_files(p15card);
	SC_TEST_RET(p15card->card->ctx, SC_LOG_DEBUG_NORMAL, r,
		"Could not add data files");

	 
	 
	r = itacns_check_and_add_keyset(p15card, "CNS0", cns0_secenv,
		0, "3F0011001101", "3F003F01", NULL,
		0x10, &found_certs);
	SC_TEST_RET(p15card->card->ctx, SC_LOG_DEBUG_NORMAL, r,
		"Could not add CNS0");
	certificate_count += found_certs;

	 
	r = itacns_check_and_add_keyset(p15card, "CNS01", 0x21,
		5, "3F002FFF8228", NULL, "3F002FFF0000",
		0x10, &found_certs);
	SC_TEST_RET(p15card->card->ctx, SC_LOG_DEBUG_NORMAL, r,
		"Could not add CNS01");
	certificate_count += found_certs;

	 
	r = itacns_check_and_add_keyset(p15card, "CNS1", 0x10,
		0, "3F0014009010", "3F00140081108010", "3F0014008110",
		0x1a, &found_certs);
	SC_TEST_RET(p15card->card->ctx, SC_LOG_DEBUG_NORMAL, r,
		"Could not add CNS1");
	certificate_count += found_certs;

	 
	if (certificate_count == 0)
		sc_debug(p15card->card->ctx, SC_LOG_DEBUG_VERBOSE,
			"Warning: no certificates found!");

	 
	sc_format_path("3F00", &path);
	r = sc_select_file(p15card->card, &path, NULL);
	SC_TEST_RET(p15card->card->ctx, SC_LOG_DEBUG_NORMAL, r,
		"Could not select master file again");

	return r;
}