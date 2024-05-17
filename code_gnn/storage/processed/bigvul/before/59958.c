static void build_feature_ctl(struct mixer_build *state, void *raw_desc,
			      unsigned int ctl_mask, int control,
			      struct usb_audio_term *iterm, int unitid,
			      int readonly_mask)
{
	struct uac_feature_unit_descriptor *desc = raw_desc;
	struct usb_feature_control_info *ctl_info;
	unsigned int len = 0;
	int mapped_name = 0;
	int nameid = uac_feature_unit_iFeature(desc);
	struct snd_kcontrol *kctl;
	struct usb_mixer_elem_info *cval;
	const struct usbmix_name_map *map;
	unsigned int range;

	control++;  

	if (control == UAC_FU_GRAPHIC_EQUALIZER) {
		 
		return;
	}

	map = find_map(state, unitid, control);
	if (check_ignored_ctl(map))
		return;

	cval = kzalloc(sizeof(*cval), GFP_KERNEL);
	if (!cval)
		return;
	snd_usb_mixer_elem_init_std(&cval->head, state->mixer, unitid);
	cval->control = control;
	cval->cmask = ctl_mask;
	ctl_info = &audio_feature_info[control-1];
	if (state->mixer->protocol == UAC_VERSION_1)
		cval->val_type = ctl_info->type;
	else  
		cval->val_type = ctl_info->type_uac2 >= 0 ?
			ctl_info->type_uac2 : ctl_info->type;

	if (ctl_mask == 0) {
		cval->channels = 1;	 
		cval->master_readonly = readonly_mask;
	} else {
		int i, c = 0;
		for (i = 0; i < 16; i++)
			if (ctl_mask & (1 << i))
				c++;
		cval->channels = c;
		cval->ch_readonly = readonly_mask;
	}

	 
	if (cval->channels == readonly_mask)
		kctl = snd_ctl_new1(&usb_feature_unit_ctl_ro, cval);
	else
		kctl = snd_ctl_new1(&usb_feature_unit_ctl, cval);

	if (!kctl) {
		usb_audio_err(state->chip, "cannot malloc kcontrol\n");
		kfree(cval);
		return;
	}
	kctl->private_free = snd_usb_mixer_elem_free;

	len = check_mapped_name(map, kctl->id.name, sizeof(kctl->id.name));
	mapped_name = len != 0;
	if (!len && nameid)
		len = snd_usb_copy_string_desc(state, nameid,
				kctl->id.name, sizeof(kctl->id.name));

	switch (control) {
	case UAC_FU_MUTE:
	case UAC_FU_VOLUME:
		 
		if (!len) {
			len = get_term_name(state, iterm, kctl->id.name,
					    sizeof(kctl->id.name), 1);
			if (!len)
				len = get_term_name(state, &state->oterm,
						    kctl->id.name,
						    sizeof(kctl->id.name), 1);
			if (!len)
				snprintf(kctl->id.name, sizeof(kctl->id.name),
					 "Feature %d", unitid);
		}

		if (!mapped_name)
			check_no_speaker_on_headset(kctl, state->mixer->chip->card);

		 
		if (!mapped_name && !(state->oterm.type >> 16)) {
			if ((state->oterm.type & 0xff00) == 0x0100)
				append_ctl_name(kctl, " Capture");
			else
				append_ctl_name(kctl, " Playback");
		}
		append_ctl_name(kctl, control == UAC_FU_MUTE ?
				" Switch" : " Volume");
		break;
	default:
		if (!len)
			strlcpy(kctl->id.name, audio_feature_info[control-1].name,
				sizeof(kctl->id.name));
		break;
	}

	 
	get_min_max_with_quirks(cval, 0, kctl);

	if (control == UAC_FU_VOLUME) {
		check_mapped_dB(map, cval);
		if (cval->dBmin < cval->dBmax || !cval->initialized) {
			kctl->tlv.c = snd_usb_mixer_vol_tlv;
			kctl->vd[0].access |=
				SNDRV_CTL_ELEM_ACCESS_TLV_READ |
				SNDRV_CTL_ELEM_ACCESS_TLV_CALLBACK;
		}
	}

	snd_usb_mixer_fu_apply_quirk(state->mixer, cval, unitid, kctl);

	range = (cval->max - cval->min) / cval->res;
	 
	if (range > 384) {
		usb_audio_warn(state->chip,
			       "Warning! Unlikely big volume range (=%u), cval->res is probably wrong.",
			       range);
		usb_audio_warn(state->chip,
			       "[%d] FU [%s] ch = %d, val = %d/%d/%d",
			       cval->head.id, kctl->id.name, cval->channels,
			       cval->min, cval->max, cval->res);
	}

	usb_audio_dbg(state->chip, "[%d] FU [%s] ch = %d, val = %d/%d/%d\n",
		      cval->head.id, kctl->id.name, cval->channels,
		      cval->min, cval->max, cval->res);
	snd_usb_mixer_add_control(&cval->head, kctl);
}