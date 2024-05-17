void snd_usb_set_format_quirk(struct snd_usb_substream *subs,
			      struct audioformat *fmt)
{
	switch (subs->stream->chip->usb_id) {
	case USB_ID(0x041e, 0x3f02):  
	case USB_ID(0x041e, 0x3f04):  
	case USB_ID(0x041e, 0x3f0a):  
	case USB_ID(0x041e, 0x3f19):  
		set_format_emu_quirk(subs, fmt);
		break;
	}
}
