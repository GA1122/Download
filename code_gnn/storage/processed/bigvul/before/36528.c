static void snd_card_set_id_no_lock(struct snd_card *card, const char *src,
				    const char *nid)
{
	int len, loops;
	bool is_default = false;
	char *id;
	
	copy_valid_id_string(card, src, nid);
	id = card->id;

 again:
	 
	if (!*id || !strncmp(id, "card", 4)) {
		strcpy(id, "Default");
		is_default = true;
	}

	len = strlen(id);
	for (loops = 0; loops < SNDRV_CARDS; loops++) {
		char *spos;
		char sfxstr[5];  
		int sfxlen;

		if (card_id_ok(card, id))
			return;  

		 
		sprintf(sfxstr, "_%X", loops + 1);
		sfxlen = strlen(sfxstr);
		if (len + sfxlen >= sizeof(card->id))
			spos = id + sizeof(card->id) - sfxlen - 1;
		else
			spos = id + len;
		strcpy(spos, sfxstr);
	}
	 
	if (!is_default) {
		*id = 0;
		goto again;
	}
	 
	dev_err(card->dev, "unable to set card id (%s)\n", id);
	if (card->proc_root->name)
		strlcpy(card->id, card->proc_root->name, sizeof(card->id));
}
