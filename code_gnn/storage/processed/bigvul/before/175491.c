static int out_set_parameters(struct audio_stream *stream, const char *kvpairs)
{
 struct stream_out *out = (struct stream_out *)stream;
 struct audio_device *adev = out->dev;
 struct audio_usecase *usecase;
 struct listnode *node;
 struct str_parms *parms;
 char value[32];
 int ret, val = 0;
 bool devices_changed;
 struct pcm_device *pcm_device;
 struct pcm_device_profile *pcm_profile;
#ifdef PREPROCESSING_ENABLED
 struct stream_in *in = NULL;  
#endif

    ALOGV("%s: enter: usecase(%d: %s) kvpairs: %s out->devices(%d) adev->mode(%d)",
          __func__, out->usecase, use_case_table[out->usecase], kvpairs, out->devices, adev->mode);
    parms = str_parms_create_str(kvpairs);
    ret = str_parms_get_str(parms, AUDIO_PARAMETER_STREAM_ROUTING, value, sizeof(value));
 if (ret >= 0) {
        val = atoi(value);
        pthread_mutex_lock(&adev->lock_inputs);
        lock_output_stream(out);
        pthread_mutex_lock(&adev->lock);
#ifdef PREPROCESSING_ENABLED
 if (((int)out->devices != val) && (val != 0) && (!out->standby) &&
 (out->usecase == USECASE_AUDIO_PLAYBACK)) {
  
 if (adev->active_input && (adev->active_input->source == AUDIO_SOURCE_VOICE_COMMUNICATION ||
                    adev->active_input->source == AUDIO_SOURCE_MIC)) {
                in = adev->active_input;
 }
 }
#endif
 if (val != 0) {
            devices_changed = out->devices != (audio_devices_t)val;
            out->devices = val;

 if (!out->standby) {
 if (devices_changed)
                    do_out_standby_l(out);
 else
                    select_devices(adev, out->usecase);
 }

 if ((adev->mode == AUDIO_MODE_IN_CALL) && !adev->in_call &&
 (out == adev->primary_output)) {
                start_voice_call(adev);
 } else if ((adev->mode == AUDIO_MODE_IN_CALL) && adev->in_call &&
 (out == adev->primary_output)) {
                select_devices(adev, USECASE_VOICE_CALL);
 }
 }

 if ((adev->mode == AUDIO_MODE_NORMAL) && adev->in_call &&
 (out == adev->primary_output)) {
            stop_voice_call(adev);
 }
        pthread_mutex_unlock(&adev->lock);
        pthread_mutex_unlock(&out->lock);
#ifdef PREPROCESSING_ENABLED
 if (in) {
  
            lock_input_stream(in);
            pthread_mutex_lock(&adev->lock);
            LOG_ALWAYS_FATAL_IF(in != adev->active_input);
            do_in_standby_l(in);
            pthread_mutex_unlock(&adev->lock);
            pthread_mutex_unlock(&in->lock);
 }
#endif
        pthread_mutex_unlock(&adev->lock_inputs);
 }

    str_parms_destroy(parms);
    ALOGV("%s: exit: code(%d)", __func__, ret);
 return ret;
}
