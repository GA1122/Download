int VirtualizerIsConfigurationSupported(audio_channel_mask_t channelMask,
 audio_devices_t deviceType) {
 uint32_t channelCount = audio_channel_count_from_out_mask(channelMask);
 if ((channelCount == 0) || (channelCount > 2)) {
 return -EINVAL;
 }

 return VirtualizerIsDeviceSupported(deviceType);
}
