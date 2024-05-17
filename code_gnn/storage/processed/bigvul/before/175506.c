static int stop_input_stream(struct stream_in *in)
{
 struct audio_usecase *uc_info;
 struct audio_device *adev = in->dev;

    adev->active_input = NULL;
    ALOGV("%s: enter: usecase(%d: %s)", __func__,
          in->usecase, use_case_table[in->usecase]);
    uc_info = get_usecase_from_id(adev, in->usecase);
 if (uc_info == NULL) {
        ALOGE("%s: Could not find the usecase (%d) in the list",
              __func__, in->usecase);
 return -EINVAL;
 }

  
    disable_snd_device(adev, uc_info, uc_info->in_snd_device, true);

    list_remove(&uc_info->adev_list_node);
    free(uc_info);

 if (list_empty(&in->pcm_dev_list)) {
        ALOGE("%s: pcm device list empty", __func__);
 return -EINVAL;
 }

    in_release_pcm_devices(in);
    list_init(&in->pcm_dev_list);

 return 0;
}
