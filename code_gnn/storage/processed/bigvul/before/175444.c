snd_device_t get_output_snd_device(struct audio_device *adev, audio_devices_t devices)
{

 audio_mode_t mode = adev->mode;
 snd_device_t snd_device = SND_DEVICE_NONE;

    ALOGV("%s: enter: output devices(%#x), mode(%d)", __func__, devices, mode);
 if (devices == AUDIO_DEVICE_NONE ||
        devices & AUDIO_DEVICE_BIT_IN) {
        ALOGV("%s: Invalid output devices (%#x)", __func__, devices);
 goto exit;
 }

 if (mode == AUDIO_MODE_IN_CALL) {
 if (devices & AUDIO_DEVICE_OUT_WIRED_HEADPHONE ||
            devices & AUDIO_DEVICE_OUT_WIRED_HEADSET) {
 if (adev->tty_mode == TTY_MODE_FULL)
                snd_device = SND_DEVICE_OUT_VOICE_TTY_FULL_HEADPHONES;
 else if (adev->tty_mode == TTY_MODE_VCO)
                snd_device = SND_DEVICE_OUT_VOICE_TTY_VCO_HEADPHONES;
 else if (adev->tty_mode == TTY_MODE_HCO)
                snd_device = SND_DEVICE_OUT_VOICE_TTY_HCO_HANDSET;
 else
                snd_device = SND_DEVICE_OUT_VOICE_HEADPHONES;
 } else if (devices & AUDIO_DEVICE_OUT_SPEAKER) {
            snd_device = SND_DEVICE_OUT_VOICE_SPEAKER;
 } else if (devices & AUDIO_DEVICE_OUT_EARPIECE) {
            snd_device = SND_DEVICE_OUT_HANDSET;
 }
 if (snd_device != SND_DEVICE_NONE) {
 goto exit;
 }
 }

 if (popcount(devices) == 2) {
 if (devices == (AUDIO_DEVICE_OUT_WIRED_HEADPHONE |
                        AUDIO_DEVICE_OUT_SPEAKER)) {
            snd_device = SND_DEVICE_OUT_SPEAKER_AND_HEADPHONES;
 } else if (devices == (AUDIO_DEVICE_OUT_WIRED_HEADSET |
                               AUDIO_DEVICE_OUT_SPEAKER)) {
            snd_device = SND_DEVICE_OUT_SPEAKER_AND_HEADPHONES;
 } else {
            ALOGE("%s: Invalid combo device(%#x)", __func__, devices);
 goto exit;
 }
 if (snd_device != SND_DEVICE_NONE) {
 goto exit;
 }
 }

 if (popcount(devices) != 1) {
        ALOGE("%s: Invalid output devices(%#x)", __func__, devices);
 goto exit;
 }

 if (devices & AUDIO_DEVICE_OUT_WIRED_HEADPHONE ||
        devices & AUDIO_DEVICE_OUT_WIRED_HEADSET) {
        snd_device = SND_DEVICE_OUT_HEADPHONES;
 } else if (devices & AUDIO_DEVICE_OUT_SPEAKER) {
        snd_device = SND_DEVICE_OUT_SPEAKER;
 } else if (devices & AUDIO_DEVICE_OUT_EARPIECE) {
        snd_device = SND_DEVICE_OUT_HANDSET;
 } else {
        ALOGE("%s: Unknown device(s) %#x", __func__, devices);
 }
exit:
    ALOGV("%s: exit: snd_device(%s)", __func__, device_table[snd_device]);
 return snd_device;
}
