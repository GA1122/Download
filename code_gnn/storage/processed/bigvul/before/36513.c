int snd_card_disconnect(struct snd_card *card)
{
	struct snd_monitor_file *mfile;
	int err;

	if (!card)
		return -EINVAL;

	spin_lock(&card->files_lock);
	if (card->shutdown) {
		spin_unlock(&card->files_lock);
		return 0;
	}
	card->shutdown = 1;
	spin_unlock(&card->files_lock);

	 
	mutex_lock(&snd_card_mutex);
	snd_cards[card->number] = NULL;
	clear_bit(card->number, snd_cards_lock);
	mutex_unlock(&snd_card_mutex);
	
	 
	
	spin_lock(&card->files_lock);
	list_for_each_entry(mfile, &card->files_list, list) {
		 
		 
		mfile->disconnected_f_op = mfile->file->f_op;

		spin_lock(&shutdown_lock);
		list_add(&mfile->shutdown_list, &shutdown_files);
		spin_unlock(&shutdown_lock);

		mfile->file->f_op = &snd_shutdown_f_ops;
		fops_get(mfile->file->f_op);
	}
	spin_unlock(&card->files_lock);	

	 
	 

#if IS_ENABLED(CONFIG_SND_MIXER_OSS)
	if (snd_mixer_oss_notify_callback)
		snd_mixer_oss_notify_callback(card, SND_MIXER_OSS_NOTIFY_DISCONNECT);
#endif

	 
	err = snd_device_disconnect_all(card);
	if (err < 0)
		dev_err(card->dev, "not all devices for card %i can be disconnected\n", card->number);

	snd_info_card_disconnect(card);
	if (card->registered) {
		device_del(&card->card_dev);
		card->registered = false;
	}
#ifdef CONFIG_PM
	wake_up(&card->power_sleep);
#endif
	return 0;	
}
