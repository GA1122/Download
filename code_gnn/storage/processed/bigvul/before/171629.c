static size_t adev_get_input_buffer_size(const struct audio_hw_device *dev,
 const struct audio_config *config)
{
    UNUSED(dev);
    UNUSED(config);

    FNLOG();

 return 320;
}
