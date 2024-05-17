static size_t get_input_buffer_size(uint32_t sample_rate,
 audio_format_t format,
 int channel_count,
 usecase_type_t usecase_type,
 audio_devices_t devices)
{
 size_t size = 0;
 struct pcm_device_profile *pcm_profile;

 if (check_input_parameters(sample_rate, format, channel_count) != 0)
 return 0;

    pcm_profile = get_pcm_device(usecase_type, devices);
 if (pcm_profile == NULL)
 return 0;

  
    size = (pcm_profile->config.period_size * sample_rate) / pcm_profile->config.rate;
    size = ((size + 15) / 16) * 16;

 return (size * channel_count * audio_bytes_per_sample(format));

}
