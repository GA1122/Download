static int parse_audio_feature_unit(struct mixer_build *state, int unitid,
				    void *_ftr)
{
	int channels, i, j;
	struct usb_audio_term iterm;
	unsigned int master_bits, first_ch_bits;
	int err, csize;
	struct uac_feature_unit_descriptor *hdr = _ftr;
	__u8 *bmaControls;

	if (state->mixer->protocol == UAC_VERSION_1) {
		csize = hdr->bControlSize;
		if (!csize) {
			usb_audio_dbg(state->chip,
				      "unit %u: invalid bControlSize == 0\n",
				      unitid);
			return -EINVAL;
		}
		channels = (hdr->bLength - 7) / csize - 1;
		bmaControls = hdr->bmaControls;
		if (hdr->bLength < 7 + csize) {
			usb_audio_err(state->chip,
				      "unit %u: invalid UAC_FEATURE_UNIT descriptor\n",
				      unitid);
			return -EINVAL;
		}
	} else {
		struct uac2_feature_unit_descriptor *ftr = _ftr;
		csize = 4;
		channels = (hdr->bLength - 6) / 4 - 1;
		bmaControls = ftr->bmaControls;
		if (hdr->bLength < 6 + csize) {
			usb_audio_err(state->chip,
				      "unit %u: invalid UAC_FEATURE_UNIT descriptor\n",
				      unitid);
			return -EINVAL;
		}
	}

	 
	if ((err = parse_audio_unit(state, hdr->bSourceID)) < 0)
		return err;

	 
	err = check_input_term(state, hdr->bSourceID, &iterm);
	if (err < 0)
		return err;

	master_bits = snd_usb_combine_bytes(bmaControls, csize);
	 
	switch (state->chip->usb_id) {
	case USB_ID(0x08bb, 0x2702):
		usb_audio_info(state->chip,
			       "usbmixer: master volume quirk for PCM2702 chip\n");
		 
		master_bits &= ~UAC_CONTROL_BIT(UAC_FU_VOLUME);
		break;
	case USB_ID(0x1130, 0xf211):
		usb_audio_info(state->chip,
			       "usbmixer: volume control quirk for Tenx TP6911 Audio Headset\n");
		 
		channels = 0;
		break;

	}
	if (channels > 0)
		first_ch_bits = snd_usb_combine_bytes(bmaControls + csize, csize);
	else
		first_ch_bits = 0;

	if (state->mixer->protocol == UAC_VERSION_1) {
		 
		for (i = 0; i < 10; i++) {
			unsigned int ch_bits = 0;
			for (j = 0; j < channels; j++) {
				unsigned int mask;

				mask = snd_usb_combine_bytes(bmaControls +
							     csize * (j+1), csize);
				if (mask & (1 << i))
					ch_bits |= (1 << j);
			}
			 

			 
			if (ch_bits & 1)
				build_feature_ctl(state, _ftr, ch_bits, i,
						  &iterm, unitid, 0);
			if (master_bits & (1 << i))
				build_feature_ctl(state, _ftr, 0, i, &iterm,
						  unitid, 0);
		}
	} else {  
		for (i = 0; i < ARRAY_SIZE(audio_feature_info); i++) {
			unsigned int ch_bits = 0;
			unsigned int ch_read_only = 0;

			for (j = 0; j < channels; j++) {
				unsigned int mask;

				mask = snd_usb_combine_bytes(bmaControls +
							     csize * (j+1), csize);
				if (uac2_control_is_readable(mask, i)) {
					ch_bits |= (1 << j);
					if (!uac2_control_is_writeable(mask, i))
						ch_read_only |= (1 << j);
				}
			}

			 

			 
			if (ch_bits & 1)
				build_feature_ctl(state, _ftr, ch_bits, i,
						  &iterm, unitid, ch_read_only);
			if (uac2_control_is_readable(master_bits, i))
				build_feature_ctl(state, _ftr, 0, i, &iterm, unitid,
						  !uac2_control_is_writeable(master_bits, i));
		}
	}

	return 0;
}