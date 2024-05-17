static int disable_output_path_l(struct stream_out *out)
{
 struct audio_device *adev = out->dev;
 struct audio_usecase *uc_info;

    uc_info = get_usecase_from_id(adev, out->usecase);
 if (uc_info == NULL) {
        ALOGE("%s: Could not find the usecase (%d) in the list",
             __func__, out->usecase);
 return -EINVAL;
 }
    disable_snd_device(adev, uc_info, uc_info->out_snd_device, true);
    uc_release_pcm_devices(uc_info);
    list_remove(&uc_info->adev_list_node);
    free(uc_info);

 return 0;
}
