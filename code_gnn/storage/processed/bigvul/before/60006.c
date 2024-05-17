static void snd_usb_mixer_interrupt_v2(struct usb_mixer_interface *mixer,
				       int attribute, int value, int index)
{
	struct usb_mixer_elem_list *list;
	__u8 unitid = (index >> 8) & 0xff;
	__u8 control = (value >> 8) & 0xff;
	__u8 channel = value & 0xff;
	unsigned int count = 0;

	if (channel >= MAX_CHANNELS) {
		usb_audio_dbg(mixer->chip,
			"%s(): bogus channel number %d\n",
			__func__, channel);
		return;
	}

	for (list = mixer->id_elems[unitid]; list; list = list->next_id_elem)
		count++;

	if (count == 0)
		return;

	for (list = mixer->id_elems[unitid]; list; list = list->next_id_elem) {
		struct usb_mixer_elem_info *info;

		if (!list->kctl)
			continue;

		info = (struct usb_mixer_elem_info *)list;
		if (count > 1 && info->control != control)
			continue;

		switch (attribute) {
		case UAC2_CS_CUR:
			 
			if (channel)
				info->cached &= ~(1 << channel);
			else  
				info->cached = 0;

			snd_ctl_notify(mixer->chip->card, SNDRV_CTL_EVENT_MASK_VALUE,
				       &info->head.kctl->id);
			break;

		case UAC2_CS_RANGE:
			 
			break;

		case UAC2_CS_MEM:
			 
			break;

		default:
			usb_audio_dbg(mixer->chip,
				"unknown attribute %d in interrupt\n",
				attribute);
			break;
		}  
	}
}