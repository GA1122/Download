static int snd_usb_create_streams(struct snd_usb_audio *chip, int ctrlif)
{
	struct usb_device *dev = chip->dev;
	struct usb_host_interface *host_iface;
	struct usb_interface_descriptor *altsd;
	int i, protocol;

	 
	host_iface = &usb_ifnum_to_if(dev, ctrlif)->altsetting[0];
	altsd = get_iface_desc(host_iface);
	protocol = altsd->bInterfaceProtocol;

	switch (protocol) {
	default:
		dev_warn(&dev->dev,
			 "unknown interface protocol %#02x, assuming v1\n",
			 protocol);
		 

	case UAC_VERSION_1: {
		struct uac1_ac_header_descriptor *h1;
		int rest_bytes;

		h1 = snd_usb_find_csint_desc(host_iface->extra,
							 host_iface->extralen,
							 NULL, UAC_HEADER);
		if (!h1) {
			dev_err(&dev->dev, "cannot find UAC_HEADER\n");
			return -EINVAL;
		}

		rest_bytes = (void *)(host_iface->extra +
				host_iface->extralen) - (void *)h1;

		 
		if (rest_bytes <= 0) {
			dev_err(&dev->dev, "invalid control header\n");
			return -EINVAL;
		}

		if (rest_bytes < sizeof(*h1)) {
			dev_err(&dev->dev, "too short v1 buffer descriptor\n");
			return -EINVAL;
		}

		if (!h1->bInCollection) {
			dev_info(&dev->dev, "skipping empty audio interface (v1)\n");
			return -EINVAL;
		}

		if (rest_bytes < h1->bLength) {
			dev_err(&dev->dev, "invalid buffer length (v1)\n");
			return -EINVAL;
		}

		if (h1->bLength < sizeof(*h1) + h1->bInCollection) {
			dev_err(&dev->dev, "invalid UAC_HEADER (v1)\n");
			return -EINVAL;
		}

		for (i = 0; i < h1->bInCollection; i++)
			snd_usb_create_stream(chip, ctrlif, h1->baInterfaceNr[i]);

		break;
	}

	case UAC_VERSION_2:
	case UAC_VERSION_3: {
		struct usb_interface_assoc_descriptor *assoc =
			usb_ifnum_to_if(dev, ctrlif)->intf_assoc;

		if (!assoc) {
			 
			struct usb_interface *iface =
				usb_ifnum_to_if(dev, ctrlif + 1);
			if (iface &&
			    iface->intf_assoc &&
			    iface->intf_assoc->bFunctionClass == USB_CLASS_AUDIO &&
			    iface->intf_assoc->bFunctionProtocol == UAC_VERSION_2)
				assoc = iface->intf_assoc;
		}

		if (!assoc) {
			dev_err(&dev->dev, "Audio class v2/v3 interfaces need an interface association\n");
			return -EINVAL;
		}

		if (protocol == UAC_VERSION_3) {
			int badd = assoc->bFunctionSubClass;

			if (badd != UAC3_FUNCTION_SUBCLASS_FULL_ADC_3_0 &&
			    (badd < UAC3_FUNCTION_SUBCLASS_GENERIC_IO ||
			     badd > UAC3_FUNCTION_SUBCLASS_SPEAKERPHONE)) {
				dev_err(&dev->dev,
					"Unsupported UAC3 BADD profile\n");
				return -EINVAL;
			}

			chip->badd_profile = badd;
		}

		for (i = 0; i < assoc->bInterfaceCount; i++) {
			int intf = assoc->bFirstInterface + i;

			if (intf != ctrlif)
				snd_usb_create_stream(chip, ctrlif, intf);
		}

		break;
	}
	}

	return 0;
}