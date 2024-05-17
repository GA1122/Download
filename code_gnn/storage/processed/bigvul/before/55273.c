int snd_usb_select_mode_quirk(struct snd_usb_substream *subs,
			      struct audioformat *fmt)
{
	struct usb_device *dev = subs->dev;
	int err;

	if (is_marantz_denon_dac(subs->stream->chip->usb_id)) {
		 
		err = usb_set_interface(dev, fmt->iface, 0);
		if (err < 0)
			return err;

		mdelay(20);  

		switch (fmt->altsetting) {
		case 2:  
		case 1:  
			err = snd_usb_ctl_msg(dev, usb_sndctrlpipe(dev, 0), 0,
					      USB_DIR_OUT|USB_TYPE_VENDOR|USB_RECIP_INTERFACE,
					      fmt->altsetting - 1, 1, NULL, 0);
			if (err < 0)
				return err;
			break;
		}
		mdelay(20);
	}
	return 0;
}
