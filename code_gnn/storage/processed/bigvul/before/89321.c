void fdt_fixup_fman_mac_addresses(void *blob)
{
	int node, i, ret;
	char *tmp, *end;
	unsigned char mac_addr[6];

	 
	tmp = env_get("ethaddr");
	if (!tmp) {
		printf("ethaddr env variable not defined\n");
		return;
	}
	for (i = 0; i < 6; i++) {
		mac_addr[i] = tmp ? simple_strtoul(tmp, &end, 16) : 0;
		if (tmp)
			tmp = (*end) ? end+1 : end;
	}

	 
	node = fdt_path_offset(blob, "/soc/fman/ethernet@e8000");
	if (node < 0) {
		printf("no /soc/fman/ethernet path offset\n");
		return;
	}
	ret = fdt_setprop(blob, node, "local-mac-address", &mac_addr, 6);
	if (ret) {
		printf("error setting local-mac-address property\n");
		return;
	}
}
