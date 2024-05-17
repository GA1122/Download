static int hid_scan_report(struct hid_device *hid)
{
	struct hid_parser *parser;
	struct hid_item item;
	__u8 *start = hid->dev_rdesc;
	__u8 *end = start + hid->dev_rsize;
	static int (*dispatch_type[])(struct hid_parser *parser,
				      struct hid_item *item) = {
		hid_scan_main,
		hid_parser_global,
		hid_parser_local,
		hid_parser_reserved
	};

	parser = vzalloc(sizeof(struct hid_parser));
	if (!parser)
		return -ENOMEM;

	parser->device = hid;
	hid->group = HID_GROUP_GENERIC;

	 
	while ((start = fetch_item(start, end, &item)) != NULL)
		dispatch_type[item.type](parser, &item);

	 
	if ((parser->scan_flags & HID_SCAN_FLAG_MT_WIN_8) &&
	    (hid->group == HID_GROUP_MULTITOUCH))
		hid->group = HID_GROUP_MULTITOUCH_WIN_8;

	 
	switch (hid->vendor) {
	case USB_VENDOR_ID_WACOM:
		hid->group = HID_GROUP_WACOM;
		break;
	case USB_VENDOR_ID_SYNAPTICS:
		if (hid->group == HID_GROUP_GENERIC)
			if ((parser->scan_flags & HID_SCAN_FLAG_VENDOR_SPECIFIC)
			    && (parser->scan_flags & HID_SCAN_FLAG_GD_POINTER))
				 
				hid->group = HID_GROUP_RMI;
		break;
	}

	vfree(parser);
	return 0;
}
