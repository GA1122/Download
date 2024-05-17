static int parse_audio_mixer_unit(struct mixer_build *state, int unitid,
				  void *raw_desc)
{
	struct uac_mixer_unit_descriptor *desc = raw_desc;
	struct usb_audio_term iterm;
	int input_pins, num_ins, num_outs;
	int pin, ich, err;

	if (desc->bLength < 11 || !(input_pins = desc->bNrInPins) ||
	    !(num_outs = uac_mixer_unit_bNrChannels(desc))) {
		usb_audio_err(state->chip,
			      "invalid MIXER UNIT descriptor %d\n",
			      unitid);
		return -EINVAL;
	}

	num_ins = 0;
	ich = 0;
	for (pin = 0; pin < input_pins; pin++) {
		err = parse_audio_unit(state, desc->baSourceID[pin]);
		if (err < 0)
			continue;
		 
		if (desc->bLength <= 10 + input_pins)
			continue;
		err = check_input_term(state, desc->baSourceID[pin], &iterm);
		if (err < 0)
			return err;
		num_ins += iterm.channels;
		for (; ich < num_ins; ich++) {
			int och, ich_has_controls = 0;

			for (och = 0; och < num_outs; och++) {
				__u8 *c = uac_mixer_unit_bmControls(desc,
						state->mixer->protocol);

				if (check_matrix_bitmap(c, ich, och, num_outs)) {
					ich_has_controls = 1;
					break;
				}
			}
			if (ich_has_controls)
				build_mixer_unit_ctl(state, desc, pin, ich,
						     unitid, &iterm);
		}
	}
	return 0;
}