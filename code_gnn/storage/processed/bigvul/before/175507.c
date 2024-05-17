static int stop_voice_call(struct audio_device *adev)
{
 struct audio_usecase *uc_info;

    ALOGV("%s: enter", __func__);
    adev->in_call = false;

  

    uc_info = get_usecase_from_id(adev, USECASE_VOICE_CALL);
 if (uc_info == NULL) {
        ALOGE("%s: Could not find the usecase (%d) in the list",
              __func__, USECASE_VOICE_CALL);
 return -EINVAL;
 }

    disable_snd_device(adev, uc_info, uc_info->out_snd_device, false);
    disable_snd_device(adev, uc_info, uc_info->in_snd_device, true);

    uc_release_pcm_devices(uc_info);
    list_remove(&uc_info->adev_list_node);
    free(uc_info);

    ALOGV("%s: exit", __func__);
 return 0;
}
