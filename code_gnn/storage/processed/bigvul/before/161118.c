void EnableHotwordEffect(const StreamControls& controls, int* effects) {
  DCHECK(effects);
  if (controls.hotword_enabled) {
#if defined(OS_CHROMEOS)
    chromeos::AudioDeviceList devices;
    chromeos::CrasAudioHandler::Get()->GetAudioDevices(&devices);
    for (const chromeos::AudioDevice& device : devices) {
      if (device.type == chromeos::AUDIO_TYPE_HOTWORD) {
        DCHECK(device.is_input);
        *effects |= media::AudioParameters::HOTWORD;
      }
    }
#endif
  }
}
