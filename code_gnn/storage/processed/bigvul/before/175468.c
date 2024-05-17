static int in_set_parameters(struct audio_stream *stream, const char *kvpairs)
{
 struct stream_in *in = (struct stream_in *)stream;
 struct audio_device *adev = in->dev;
 struct str_parms *parms;
 char *str;
 char value[32];
 int ret, val = 0;
 struct audio_usecase *uc_info;
 bool do_standby = false;
 struct listnode *node;
 struct pcm_device *pcm_device;
 struct pcm_device_profile *pcm_profile;

    ALOGV("%s: enter: kvpairs=%s", __func__, kvpairs);
    parms = str_parms_create_str(kvpairs);

    ret = str_parms_get_str(parms, AUDIO_PARAMETER_STREAM_INPUT_SOURCE, value, sizeof(value));

    pthread_mutex_lock(&adev->lock_inputs);
    lock_input_stream(in);
    pthread_mutex_lock(&adev->lock);
 if (ret >= 0) {
        val = atoi(value);
  
 if (((int)in->source != val) && (val != 0)) {
            in->source = val;
 }
 }

    ret = str_parms_get_str(parms, AUDIO_PARAMETER_STREAM_ROUTING, value, sizeof(value));
 if (ret >= 0) {
        val = atoi(value);
 if (((int)in->devices != val) && (val != 0)) {
            in->devices = val;
  
 if (!in->standby) {
                uc_info = get_usecase_from_id(adev, in->usecase);
 if (uc_info == NULL) {
                    ALOGE("%s: Could not find the usecase (%d) in the list",
                          __func__, in->usecase);
 } else {
 if (list_empty(&in->pcm_dev_list))
                        ALOGE("%s: pcm device list empty", __func__);
 else {
                        pcm_device = node_to_item(list_head(&in->pcm_dev_list),
 struct pcm_device, stream_list_node);
 if ((pcm_device->pcm_profile->devices & val & ~AUDIO_DEVICE_BIT_IN) == 0) {
                            do_standby = true;
 }
 }
 }
 if (do_standby) {
                    ret = do_in_standby_l(in);
 } else
                    ret = select_devices(adev, in->usecase);
 }
 }
 }
    pthread_mutex_unlock(&adev->lock);
    pthread_mutex_unlock(&in->lock);
    pthread_mutex_unlock(&adev->lock_inputs);
    str_parms_destroy(parms);

 if (ret > 0)
        ret = 0;

 return ret;
}
