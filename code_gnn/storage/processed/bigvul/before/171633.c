static int adev_open(const hw_module_t* module, const char* name,
 hw_device_t** device)
{
 struct a2dp_audio_device *adev;
 int ret;

    INFO(" adev_open in A2dp_hw module");
    FNLOG();

 if (strcmp(name, AUDIO_HARDWARE_INTERFACE) != 0)
 {
        ERROR("interface %s not matching [%s]", name, AUDIO_HARDWARE_INTERFACE);
 return -EINVAL;
 }

    adev = calloc(1, sizeof(struct a2dp_audio_device));

 if (!adev)
 return -ENOMEM;

    adev->device.common.tag = HARDWARE_DEVICE_TAG;
    adev->device.common.version = AUDIO_DEVICE_API_VERSION_2_0;
    adev->device.common.module = (struct hw_module_t *) module;
    adev->device.common.close = adev_close;

    adev->device.init_check = adev_init_check;
    adev->device.set_voice_volume = adev_set_voice_volume;
    adev->device.set_master_volume = adev_set_master_volume;
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

    adev->output = NULL;


 *device = &adev->device.common;

 return 0;
}
