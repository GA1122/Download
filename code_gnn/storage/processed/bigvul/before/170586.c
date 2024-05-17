int VirtualizerIsDeviceSupported(audio_devices_t deviceType) {
 switch (deviceType) {
 case AUDIO_DEVICE_OUT_WIRED_HEADSET:
 case AUDIO_DEVICE_OUT_WIRED_HEADPHONE:
 case AUDIO_DEVICE_OUT_BLUETOOTH_A2DP_HEADPHONES:
 return 0;
 default :
 return -EINVAL;
 }
}
