static int disable_snd_device(struct audio_device *adev,
 struct audio_usecase *uc_info,
 snd_device_t snd_device,
 bool update_mixer)
{
 struct mixer_card *mixer_card;
 struct listnode *node;
 const char *snd_device_name = get_snd_device_name(snd_device);

 if (snd_device_name == NULL)
 return -EINVAL;

 if (adev->snd_dev_ref_cnt[snd_device] <= 0) {
        ALOGE("%s: device ref cnt is already 0", __func__);
 return -EINVAL;
 }
    adev->snd_dev_ref_cnt[snd_device]--;
 if (adev->snd_dev_ref_cnt[snd_device] == 0) {
        ALOGV("%s: snd_device(%d: %s)", __func__,
              snd_device, snd_device_name);
        list_for_each(node, &uc_info->mixer_list) {
            mixer_card = node_to_item(node, struct mixer_card, uc_list_node[uc_info->id]);
            audio_route_reset_path(mixer_card->audio_route, snd_device_name);
 if (update_mixer)
                audio_route_update_mixer(mixer_card->audio_route);
 }
 }
 return 0;
}
