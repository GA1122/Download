static int adev_set_parameters(struct audio_hw_device *dev, const char *kvpairs)
{
 struct audio_device *adev = (struct audio_device *)dev;
 struct str_parms *parms;
 char *str;
 char value[32];
 int val;
 int ret;

    ALOGV("%s: enter: %s", __func__, kvpairs);

    parms = str_parms_create_str(kvpairs);
    ret = str_parms_get_str(parms, AUDIO_PARAMETER_KEY_TTY_MODE, value, sizeof(value));
 if (ret >= 0) {
 int tty_mode;

 if (strcmp(value, AUDIO_PARAMETER_VALUE_TTY_OFF) == 0)
            tty_mode = TTY_MODE_OFF;
 else if (strcmp(value, AUDIO_PARAMETER_VALUE_TTY_VCO) == 0)
            tty_mode = TTY_MODE_VCO;
 else if (strcmp(value, AUDIO_PARAMETER_VALUE_TTY_HCO) == 0)
            tty_mode = TTY_MODE_HCO;
 else if (strcmp(value, AUDIO_PARAMETER_VALUE_TTY_FULL) == 0)
            tty_mode = TTY_MODE_FULL;
 else
 return -EINVAL;

        pthread_mutex_lock(&adev->lock);
 if (tty_mode != adev->tty_mode) {
            adev->tty_mode = tty_mode;
 if (adev->in_call)
                select_devices(adev, USECASE_VOICE_CALL);
 }
        pthread_mutex_unlock(&adev->lock);
 }

    ret = str_parms_get_str(parms, AUDIO_PARAMETER_KEY_BT_NREC, value, sizeof(value));
 if (ret >= 0) {
  
 if (strcmp(value, AUDIO_PARAMETER_VALUE_ON) == 0)
            adev->bluetooth_nrec = true;
 else
            adev->bluetooth_nrec = false;
 }

    ret = str_parms_get_str(parms, "screen_state", value, sizeof(value));
 if (ret >= 0) {
 if (strcmp(value, AUDIO_PARAMETER_VALUE_ON) == 0)
            adev->screen_off = false;
 else
            adev->screen_off = true;
 }

    ret = str_parms_get_int(parms, "rotation", &val);
 if (ret >= 0) {
 bool reverse_speakers = false;
 switch(val) {
  
 case 180:
            reverse_speakers = true;
 break;
 case 0:
 case 90:
 case 270:
 break;
 default:
            ALOGE("%s: unexpected rotation of %d", __func__, val);
 }
        pthread_mutex_lock(&adev->lock);
 if (adev->speaker_lr_swap != reverse_speakers) {
            adev->speaker_lr_swap = reverse_speakers;
 struct mixer_card *mixer_card;
            mixer_card = adev_get_mixer_for_card(adev, SOUND_CARD);
 if (mixer_card)
                audio_route_apply_and_update_path(mixer_card->audio_route,
                        reverse_speakers ? "speaker-lr-reverse" :
 "speaker-lr-normal");
 }
        pthread_mutex_unlock(&adev->lock);
 }

    str_parms_destroy(parms);
    ALOGV("%s: exit with code(%d)", __func__, ret);
 return ret;
}
