static int detect_idkey(
	sc_pkcs15_card_t *p15card
){
	sc_card_t *card=p15card->card;
	sc_path_t p;

	 
	memset(&p, 0, sizeof(sc_path_t));
	p.type=SC_PATH_TYPE_DF_NAME;
	memcpy(p.value, "\xD2\x76\x00\x00\x03\x0C\x01", p.len=7);
	if (sc_select_file(card,&p,NULL)!=SC_SUCCESS) return 1;

	p15card->tokeninfo->manufacturer_id = strdup("TeleSec GmbH");
	p15card->tokeninfo->label = strdup("IDKey Card");

	insert_cert(p15card, "DF074331", 0x45, 1, "Signatur Zertifikat 1");
	insert_cert(p15card, "DF074332", 0x45, 1, "Signatur Zertifikat 2");
	insert_cert(p15card, "DF074333", 0x45, 1, "Signatur Zertifikat 3");

	insert_key(p15card, "DF074E03", 0x45, 0x84, 2048, 1, "IDKey1");
	insert_key(p15card, "DF074E04", 0x46, 0x85, 2048, 1, "IDKey2");
	insert_key(p15card, "DF074E05", 0x47, 0x86, 2048, 1, "IDKey3");
	insert_key(p15card, "DF074E06", 0x48, 0x87, 2048, 1, "IDKey4");
	insert_key(p15card, "DF074E07", 0x49, 0x88, 2048, 1, "IDKey5");
	insert_key(p15card, "DF074E08", 0x4A, 0x89, 2048, 1, "IDKey6");

	insert_pin(p15card, "5000", 1, 2, 0x00, 6, "PIN",
		SC_PKCS15_PIN_FLAG_CASE_SENSITIVE | SC_PKCS15_PIN_FLAG_INITIALIZED
	);
	insert_pin(p15card, "5001", 2, 0, 0x01, 8, "PUK",
		SC_PKCS15_PIN_FLAG_CASE_SENSITIVE | SC_PKCS15_PIN_FLAG_INITIALIZED |
		SC_PKCS15_PIN_FLAG_UNBLOCKING_PIN | SC_PKCS15_PIN_FLAG_SO_PIN
	);

	return 0;
}