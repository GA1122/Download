static int parse_audio_unit(struct mixer_build *state, int unitid)
{
	unsigned char *p1;

	if (test_and_set_bit(unitid, state->unitbitmap))
		return 0;  

	p1 = find_audio_control_unit(state, unitid);
	if (!p1) {
		usb_audio_err(state->chip, "unit %d not found!\n", unitid);
		return -EINVAL;
	}

	switch (p1[2]) {
	case UAC_INPUT_TERMINAL:
		return 0;  
	case UAC_MIXER_UNIT:
		return parse_audio_mixer_unit(state, unitid, p1);
	case UAC2_CLOCK_SOURCE:
		return parse_clock_source_unit(state, unitid, p1);
	case UAC_SELECTOR_UNIT:
	case UAC2_CLOCK_SELECTOR:
		return parse_audio_selector_unit(state, unitid, p1);
	case UAC_FEATURE_UNIT:
		return parse_audio_feature_unit(state, unitid, p1);
	case UAC1_PROCESSING_UNIT:
	 
		if (state->mixer->protocol == UAC_VERSION_1)
			return parse_audio_processing_unit(state, unitid, p1);
		else
			return 0;  
	case UAC1_EXTENSION_UNIT:
	 
		if (state->mixer->protocol == UAC_VERSION_1)
			return parse_audio_extension_unit(state, unitid, p1);
		else  
			return parse_audio_processing_unit(state, unitid, p1);
	case UAC2_EXTENSION_UNIT_V2:
		return parse_audio_extension_unit(state, unitid, p1);
	default:
		usb_audio_err(state->chip,
			"unit %u: unexpected type 0x%02x\n", unitid, p1[2]);
		return -EINVAL;
	}
}