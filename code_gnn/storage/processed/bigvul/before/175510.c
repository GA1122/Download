static int uc_select_pcm_devices(struct audio_usecase *usecase)

{
 struct stream_out *out = (struct stream_out *)usecase->stream;
 struct pcm_device *pcm_device;
 struct pcm_device_profile *pcm_profile;
 struct mixer_card *mixer_card;
 audio_devices_t devices = usecase->devices;

    list_init(&usecase->mixer_list);
    list_init(&out->pcm_dev_list);

    pcm_profile = get_pcm_device(usecase->type, devices);
 if (pcm_profile) {
        pcm_device = calloc(1, sizeof(struct pcm_device));
        pcm_device->pcm_profile = pcm_profile;
        list_add_tail(&out->pcm_dev_list, &pcm_device->stream_list_node);
        mixer_card = uc_get_mixer_for_card(usecase, pcm_profile->card);
 if (mixer_card == NULL) {
            mixer_card = adev_get_mixer_for_card(out->dev, pcm_profile->card);
            list_add_tail(&usecase->mixer_list, &mixer_card->uc_list_node[usecase->id]);
 }
        devices &= ~pcm_profile->devices;
 } else {
        ALOGE("usecase type=%d, devices=%d did not find exact match",
            usecase->type, devices);
 }

 return 0;
}
