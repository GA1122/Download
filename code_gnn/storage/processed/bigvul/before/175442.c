snd_device_t get_input_snd_device(struct audio_device *adev, audio_devices_t out_device)
{
 audio_source_t  source;
 audio_mode_t    mode   = adev->mode;
 audio_devices_t in_device;
 audio_channel_mask_t channel_mask;
 snd_device_t snd_device = SND_DEVICE_NONE;
 struct stream_in *active_input = NULL;
 struct audio_usecase *usecase;

    usecase = get_usecase_from_type(adev, PCM_CAPTURE|VOICE_CALL);
 if (usecase != NULL) {
        active_input = (struct stream_in *)usecase->stream;
 }
    source = (active_input == NULL) ?
                                AUDIO_SOURCE_DEFAULT : active_input->source;

    in_device = ((active_input == NULL) ?
                                    AUDIO_DEVICE_NONE : active_input->devices)
 & ~AUDIO_DEVICE_BIT_IN;
    channel_mask = (active_input == NULL) ?
                                AUDIO_CHANNEL_IN_MONO : active_input->main_channels;

    ALOGV("%s: enter: out_device(%#x) in_device(%#x)",
          __func__, out_device, in_device);
 if (mode == AUDIO_MODE_IN_CALL) {
 if (out_device == AUDIO_DEVICE_NONE) {
            ALOGE("%s: No output device set for voice call", __func__);
 goto exit;
 }
 if (adev->tty_mode != TTY_MODE_OFF) {
 if (out_device & AUDIO_DEVICE_OUT_WIRED_HEADPHONE ||
                out_device & AUDIO_DEVICE_OUT_WIRED_HEADSET) {
 switch (adev->tty_mode) {
 case TTY_MODE_FULL:
                    snd_device = SND_DEVICE_IN_VOICE_TTY_FULL_HEADSET_MIC;
 break;
 case TTY_MODE_VCO:
                    snd_device = SND_DEVICE_IN_VOICE_TTY_VCO_HANDSET_MIC;
 break;
 case TTY_MODE_HCO:
                    snd_device = SND_DEVICE_IN_VOICE_TTY_HCO_HEADSET_MIC;
 break;
 default:
                    ALOGE("%s: Invalid TTY mode (%#x)", __func__, adev->tty_mode);
 }
 goto exit;
 }
 }
 if (out_device & AUDIO_DEVICE_OUT_EARPIECE ||
                out_device & AUDIO_DEVICE_OUT_WIRED_HEADPHONE) {
            snd_device = SND_DEVICE_IN_HANDSET_MIC;
 } else if (out_device & AUDIO_DEVICE_OUT_WIRED_HEADSET) {
            snd_device = SND_DEVICE_IN_VOICE_HEADSET_MIC;
 } else if (out_device & AUDIO_DEVICE_OUT_SPEAKER) {
            snd_device = SND_DEVICE_IN_VOICE_SPEAKER_MIC;
 }
 } else if (source == AUDIO_SOURCE_CAMCORDER) {
 if (in_device & AUDIO_DEVICE_IN_BUILTIN_MIC ||
            in_device & AUDIO_DEVICE_IN_BACK_MIC) {
            snd_device = SND_DEVICE_IN_CAMCORDER_MIC;
 }
 } else if (source == AUDIO_SOURCE_VOICE_RECOGNITION) {
 if (in_device & AUDIO_DEVICE_IN_BUILTIN_MIC) {
 if (adev->dualmic_config == DUALMIC_CONFIG_1) {
 if (channel_mask == AUDIO_CHANNEL_IN_FRONT_BACK)
                    snd_device = SND_DEVICE_IN_VOICE_REC_DMIC_1;
 else if (adev->ns_in_voice_rec)
                    snd_device = SND_DEVICE_IN_VOICE_REC_DMIC_NS_1;
 }

 if (snd_device == SND_DEVICE_NONE) {
                snd_device = SND_DEVICE_IN_VOICE_REC_MIC;
 }
 } else if (in_device & AUDIO_DEVICE_IN_WIRED_HEADSET) {
            snd_device = SND_DEVICE_IN_VOICE_REC_HEADSET_MIC;
 }
 } else if (source == AUDIO_SOURCE_VOICE_COMMUNICATION || source == AUDIO_SOURCE_MIC) {
 if (out_device & AUDIO_DEVICE_OUT_SPEAKER)
            in_device = AUDIO_DEVICE_IN_BACK_MIC;
 if (active_input) {
 if (active_input->enable_aec) {
 if (in_device & AUDIO_DEVICE_IN_BACK_MIC) {
                    snd_device = SND_DEVICE_IN_SPEAKER_MIC_AEC;
 } else if (in_device & AUDIO_DEVICE_IN_BUILTIN_MIC) {
 if (out_device & AUDIO_DEVICE_OUT_WIRED_HEADPHONE) {
                        snd_device = SND_DEVICE_IN_SPEAKER_MIC_AEC;
 } else {
                        snd_device = SND_DEVICE_IN_HANDSET_MIC_AEC;
 }
 } else if (in_device & AUDIO_DEVICE_IN_WIRED_HEADSET) {
                    snd_device = SND_DEVICE_IN_HEADSET_MIC_AEC;
 }
 }
  
 }
 } else if (source == AUDIO_SOURCE_DEFAULT) {
 goto exit;
 }


 if (snd_device != SND_DEVICE_NONE) {
 goto exit;
 }

 if (in_device != AUDIO_DEVICE_NONE &&
 !(in_device & AUDIO_DEVICE_IN_VOICE_CALL) &&
 !(in_device & AUDIO_DEVICE_IN_COMMUNICATION)) {
 if (in_device & AUDIO_DEVICE_IN_BUILTIN_MIC) {
            snd_device = SND_DEVICE_IN_HANDSET_MIC;
 } else if (in_device & AUDIO_DEVICE_IN_BACK_MIC) {
            snd_device = SND_DEVICE_IN_SPEAKER_MIC;
 } else if (in_device & AUDIO_DEVICE_IN_WIRED_HEADSET) {
            snd_device = SND_DEVICE_IN_HEADSET_MIC;
 } else if (in_device & AUDIO_DEVICE_IN_AUX_DIGITAL) {
            snd_device = SND_DEVICE_IN_HDMI_MIC;
 } else {
            ALOGE("%s: Unknown input device(s) %#x", __func__, in_device);
            ALOGW("%s: Using default handset-mic", __func__);
            snd_device = SND_DEVICE_IN_HANDSET_MIC;
 }
 } else {
 if (out_device & AUDIO_DEVICE_OUT_EARPIECE) {
            snd_device = SND_DEVICE_IN_HANDSET_MIC;
 } else if (out_device & AUDIO_DEVICE_OUT_WIRED_HEADSET) {
            snd_device = SND_DEVICE_IN_HEADSET_MIC;
 } else if (out_device & AUDIO_DEVICE_OUT_SPEAKER) {
            snd_device = SND_DEVICE_IN_SPEAKER_MIC;
 } else if (out_device & AUDIO_DEVICE_OUT_WIRED_HEADPHONE) {
            snd_device = SND_DEVICE_IN_HANDSET_MIC;
 } else {
            ALOGE("%s: Unknown output device(s) %#x", __func__, out_device);
            ALOGW("%s: Using default handset-mic", __func__);
            snd_device = SND_DEVICE_IN_HANDSET_MIC;
 }
 }
exit:
    ALOGV("%s: exit: in_snd_device(%s)", __func__, device_table[snd_device]);
 return snd_device;
}
