int snd_usb_mixer_resume(struct usb_mixer_interface *mixer, bool reset_resume)
{
	struct usb_mixer_elem_list *list;
	int id, err;

	if (reset_resume) {
		 
		for (id = 0; id < MAX_ID_ELEMS; id++) {
			for (list = mixer->id_elems[id]; list;
			     list = list->next_id_elem) {
				if (list->resume) {
					err = list->resume(list);
					if (err < 0)
						return err;
				}
			}
		}
	}

	return snd_usb_mixer_activate(mixer);
}