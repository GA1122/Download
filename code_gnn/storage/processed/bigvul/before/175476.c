int mixer_init(struct audio_device *adev)
{
 int i;
 int card;
 int retry_num;
 struct mixer *mixer;
 struct audio_route *audio_route;
 char mixer_path[PATH_MAX];
 struct mixer_card *mixer_card;
 struct listnode *node;

    list_init(&adev->mixer_list);

 for (i = 0; pcm_devices[i] != NULL; i++) {
        card = pcm_devices[i]->card;
 if (adev_get_mixer_for_card(adev, card) == NULL) {
            retry_num = 0;
 do {
                mixer = mixer_open(card);
 if (mixer == NULL) {
 if (++retry_num > RETRY_NUMBER) {
                        ALOGE("%s unable to open the mixer for--card %d, aborting.",
                              __func__, card);
 goto error;
 }
                    usleep(RETRY_US);
 }
 } while (mixer == NULL);

            sprintf(mixer_path, "/system/etc/mixer_paths_%d.xml", card);
            audio_route = audio_route_init(card, mixer_path);
 if (!audio_route) {
                ALOGE("%s: Failed to init audio route controls for card %d, aborting.",
                      __func__, card);
 goto error;
 }
            mixer_card = calloc(1, sizeof(struct mixer_card));
            mixer_card->card = card;
            mixer_card->mixer = mixer;
            mixer_card->audio_route = audio_route;
            list_add_tail(&adev->mixer_list, &mixer_card->adev_list_node);
 }
 }

 return 0;

error:
    free_mixer_list(adev);
 return -ENODEV;
}
