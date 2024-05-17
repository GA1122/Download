static int print_udev_ambivalent(blkid_probe pr)
{
	char *val = NULL;
	size_t valsz = 0;
	int count = 0, rc = -1;

	while (!blkid_do_probe(pr)) {
		const char *usage_txt = NULL, *type = NULL, *version = NULL;
		char enc[256];

		blkid_probe_lookup_value(pr, "USAGE", &usage_txt, NULL);
		blkid_probe_lookup_value(pr, "TYPE", &type, NULL);
		blkid_probe_lookup_value(pr, "VERSION", &version, NULL);

		if (!usage_txt || !type)
			continue;

		blkid_encode_string(usage_txt, enc, sizeof(enc));
		if (append_str(&val, &valsz, enc, ":"))
			goto done;

		blkid_encode_string(type, enc, sizeof(enc));
		if (append_str(&val, &valsz, enc, version ? ":" : " "))
			goto done;

		if (version) {
			blkid_encode_string(version, enc, sizeof(enc));
			if (append_str(&val, &valsz, enc, " "))
				goto done;
		}
		count++;
	}

	if (count > 1) {
		*(val + valsz - 1) = '\0';		 
		printf("ID_FS_AMBIVALENT=%s\n", val);
		rc = 0;
	}
done:
	free(val);
	return rc;
}