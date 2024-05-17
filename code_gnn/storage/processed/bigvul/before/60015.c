static void volume_control_quirks(struct usb_mixer_elem_info *cval,
				  struct snd_kcontrol *kctl)
{
	struct snd_usb_audio *chip = cval->head.mixer->chip;
	switch (chip->usb_id) {
	case USB_ID(0x0763, 0x2030):  
	case USB_ID(0x0763, 0x2031):  
		if (strcmp(kctl->id.name, "Effect Duration") == 0) {
			cval->min = 0x0000;
			cval->max = 0xffff;
			cval->res = 0x00e6;
			break;
		}
		if (strcmp(kctl->id.name, "Effect Volume") == 0 ||
		    strcmp(kctl->id.name, "Effect Feedback Volume") == 0) {
			cval->min = 0x00;
			cval->max = 0xff;
			break;
		}
		if (strstr(kctl->id.name, "Effect Return") != NULL) {
			cval->min = 0xb706;
			cval->max = 0xff7b;
			cval->res = 0x0073;
			break;
		}
		if ((strstr(kctl->id.name, "Playback Volume") != NULL) ||
			(strstr(kctl->id.name, "Effect Send") != NULL)) {
			cval->min = 0xb5fb;  
			cval->max = 0xfcfe;
			cval->res = 0x0073;
		}
		break;

	case USB_ID(0x0763, 0x2081):  
	case USB_ID(0x0763, 0x2080):  
		if (strcmp(kctl->id.name, "Effect Duration") == 0) {
			usb_audio_info(chip,
				       "set quirk for FTU Effect Duration\n");
			cval->min = 0x0000;
			cval->max = 0x7f00;
			cval->res = 0x0100;
			break;
		}
		if (strcmp(kctl->id.name, "Effect Volume") == 0 ||
		    strcmp(kctl->id.name, "Effect Feedback Volume") == 0) {
			usb_audio_info(chip,
				       "set quirks for FTU Effect Feedback/Volume\n");
			cval->min = 0x00;
			cval->max = 0x7f;
			break;
		}
		break;

	case USB_ID(0x0471, 0x0101):
	case USB_ID(0x0471, 0x0104):
	case USB_ID(0x0471, 0x0105):
	case USB_ID(0x0672, 0x1041):
	 
		if (!strcmp(kctl->id.name, "PCM Playback Volume") &&
		    cval->min == -15616) {
			usb_audio_info(chip,
				 "set volume quirk for UDA1321/N101 chip\n");
			cval->max = -256;
		}
		break;

	case USB_ID(0x046d, 0x09a4):
		if (!strcmp(kctl->id.name, "Mic Capture Volume")) {
			usb_audio_info(chip,
				"set volume quirk for QuickCam E3500\n");
			cval->min = 6080;
			cval->max = 8768;
			cval->res = 192;
		}
		break;

	case USB_ID(0x046d, 0x0807):  
	case USB_ID(0x046d, 0x0808):
	case USB_ID(0x046d, 0x0809):
	case USB_ID(0x046d, 0x0819):  
	case USB_ID(0x046d, 0x081b):  
	case USB_ID(0x046d, 0x081d):  
	case USB_ID(0x046d, 0x0825):  
	case USB_ID(0x046d, 0x0826):  
	case USB_ID(0x046d, 0x08ca):  
	case USB_ID(0x046d, 0x0991):
	case USB_ID(0x046d, 0x09a2):  
	 
		if (!strcmp(kctl->id.name, "Mic Capture Volume")) {
			usb_audio_info(chip,
				"set resolution quirk: cval->res = 384\n");
			cval->res = 384;
		}
		break;
	}
}
