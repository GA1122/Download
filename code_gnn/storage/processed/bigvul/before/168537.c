static void get_api_type(struct libusb_context *ctx, HDEVINFO *dev_info,
	SP_DEVINFO_DATA *dev_info_data, int *api, int *sub_api)
{
	struct driver_lookup lookup[3] = {
		{"\0\0", SPDRP_SERVICE, "driver"},
		{"\0\0", SPDRP_UPPERFILTERS, "upper filter driver"},
		{"\0\0", SPDRP_LOWERFILTERS, "lower filter driver"}
	};
	DWORD size, reg_type;
	unsigned k, l;
	int i, j;

	*api = USB_API_UNSUPPORTED;
	*sub_api = SUB_API_NOTSET;
	for (k=0; k<3; k++) {
		if (pSetupDiGetDeviceRegistryPropertyA(*dev_info, dev_info_data, lookup[k].reg_prop,
			&reg_type, (BYTE*)lookup[k].list, MAX_KEY_LENGTH, &size)) {
			if (lookup[k].reg_prop == SPDRP_SERVICE) {
				lookup[k].list[safe_strlen(lookup[k].list)+1] = 0;
			}
			for (l=0; (lookup[k].list[l] != 0) || (lookup[k].list[l+1] != 0); l++) {
				if (lookup[k].list[l] == 0) {
					lookup[k].list[l] = LIST_SEPARATOR;
				}
			}
			usbi_dbg("%s(s): %s", lookup[k].designation, lookup[k].list);
		} else {
			if (GetLastError() != ERROR_INVALID_DATA) {
				usbi_dbg("could not access %s: %s", lookup[k].designation, windows_error_str(0));
			}
			lookup[k].list[0] = 0;
		}
	}

	for (i=1; i<USB_API_MAX; i++) {
		for (k=0; k<3; k++) {
			j = get_sub_api(lookup[k].list, i);
			if (j >= 0) {
				usbi_dbg("matched %s name against %s API", 
					lookup[k].designation, (i!=USB_API_WINUSBX)?usb_api_backend[i].designation:sub_api_name[j]);
				*api = i;
				*sub_api = j;
				return;
			}
		}
	}
}
