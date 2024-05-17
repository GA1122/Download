u64 snd_usb_interface_dsd_format_quirks(struct snd_usb_audio *chip,
					struct audioformat *fp,
					unsigned int sample_bytes)
{
	 
	if (USB_ID_VENDOR(chip->usb_id) == 0x23ba) {
		switch (fp->altsetting) {
		case 1:
			fp->dsd_dop = true;
			return SNDRV_PCM_FMTBIT_DSD_U16_LE;
		case 2:
			fp->dsd_bitrev = true;
			return SNDRV_PCM_FMTBIT_DSD_U8;
		case 3:
			fp->dsd_bitrev = true;
			return SNDRV_PCM_FMTBIT_DSD_U16_LE;
		}
	}

	 
	switch (chip->usb_id) {
	case USB_ID(0x20b1, 0x3008):  
	case USB_ID(0x20b1, 0x2008):  
	case USB_ID(0x20b1, 0x300a):  
	case USB_ID(0x22d9, 0x0416):  
		if (fp->altsetting == 2)
			return SNDRV_PCM_FMTBIT_DSD_U32_BE;
		break;

	case USB_ID(0x20b1, 0x000a):  
	case USB_ID(0x20b1, 0x2009):  
	case USB_ID(0x20b1, 0x2023):  
	case USB_ID(0x20b1, 0x3023):  
	case USB_ID(0x2616, 0x0106):  
		if (fp->altsetting == 3)
			return SNDRV_PCM_FMTBIT_DSD_U32_BE;
		break;
	default:
		break;
	}

	 
	if (is_marantz_denon_dac(chip->usb_id)) {
		if (fp->altsetting == 2)
			return SNDRV_PCM_FMTBIT_DSD_U32_BE;
	}

	return 0;
}
