static int adev_open(const hw_module_t *module, const char *name,
 hw_device_t **device)
{
 struct audio_device *adev;
 int i, ret, retry_count;

    ALOGV("%s: enter", __func__);
 if (strcmp(name, AUDIO_HARDWARE_INTERFACE) != 0) return -EINVAL;

    adev = calloc(1, sizeof(struct audio_device));

    adev->device.common.tag = HARDWARE_DEVICE_TAG;
    adev->device.common.version = AUDIO_DEVICE_API_VERSION_2_0;
    adev->device.common.module = (struct hw_module_t *)module;
    adev->device.common.close = adev_close;

    adev->device.init_check = adev_init_check;
    adev->device.set_voice_volume = adev_set_voice_volume;
    adev->device.set_master_volume = adev_set_master_volume;
    adev->device.get_master_volume = adev_get_master_volume;
    adev->device.set_master_mute = adev_set_master_mute;
    adev->device.get_master_mute = adev_get_master_mute;
    adev->device.set_mode = adev_set_mode;
    adev->device.set_mic_mute = adev_set_mic_mute;
    adev->device.get_mic_mute = adev_get_mic_mute;
    adev->device.set_parameters = adev_set_parameters;
    adev->device.get_parameters = adev_get_parameters;
    adev->device.get_input_buffer_size = adev_get_input_buffer_size;
    adev->device.open_output_stream = adev_open_output_stream;
    adev->device.close_output_stream = adev_close_output_stream;
    adev->device.open_input_stream = adev_open_input_stream;
    adev->device.close_input_stream = adev_close_input_stream;
    adev->device.dump = adev_dump;

  
    adev->mode = AUDIO_MODE_NORMAL;
    adev->active_input = NULL;
    adev->primary_output = NULL;
    adev->voice_volume = 1.0f;
    adev->tty_mode = TTY_MODE_OFF;
    adev->bluetooth_nrec = true;
    adev->in_call = false;
  
    adev->snd_dev_ref_cnt = calloc(SND_DEVICE_MAX, sizeof(int));

    adev->dualmic_config = DUALMIC_CONFIG_NONE;
    adev->ns_in_voice_rec = false;

    list_init(&adev->usecase_list);

 if (mixer_init(adev) != 0) {
        free(adev->snd_dev_ref_cnt);
        free(adev);
        ALOGE("%s: Failed to init, aborting.", __func__);
 *device = NULL;
 return -EINVAL;
 }


 if (access(SOUND_TRIGGER_HAL_LIBRARY_PATH, R_OK) == 0) {
        adev->sound_trigger_lib = dlopen(SOUND_TRIGGER_HAL_LIBRARY_PATH,
                                         RTLD_NOW);
 if (adev->sound_trigger_lib == NULL) {
            ALOGE("%s: DLOPEN failed for %s", __func__,
                  SOUND_TRIGGER_HAL_LIBRARY_PATH);
 } else {
            ALOGV("%s: DLOPEN successful for %s", __func__,
                  SOUND_TRIGGER_HAL_LIBRARY_PATH);
            adev->sound_trigger_open_for_streaming =
 (int (*)(void))dlsym(adev->sound_trigger_lib,
 "sound_trigger_open_for_streaming");
            adev->sound_trigger_read_samples =
 (size_t (*)(int, void *, size_t))dlsym(
                            adev->sound_trigger_lib,
 "sound_trigger_read_samples");
            adev->sound_trigger_close_for_streaming =
 (int (*)(int))dlsym(
                                adev->sound_trigger_lib,
 "sound_trigger_close_for_streaming");
 if (!adev->sound_trigger_open_for_streaming ||
 !adev->sound_trigger_read_samples ||
 !adev->sound_trigger_close_for_streaming) {

                ALOGE("%s: Error grabbing functions in %s", __func__,
                      SOUND_TRIGGER_HAL_LIBRARY_PATH);
                adev->sound_trigger_open_for_streaming = 0;
                adev->sound_trigger_read_samples = 0;
                adev->sound_trigger_close_for_streaming = 0;
 }
 }
 }

 *device = &adev->device.common;

    cras_dsp_init("/system/etc/cras/speakerdsp.ini");

    ALOGV("%s: exit", __func__);
 return 0;
}
