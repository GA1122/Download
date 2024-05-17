static void adev_close_input_stream(struct audio_hw_device *dev,
 struct audio_stream_in *stream)
{
 struct audio_device *adev = (struct audio_device *)dev;
 struct stream_in *in = (struct stream_in*)stream;
    ALOGV("%s", __func__);

  
    pthread_mutex_lock(&adev->lock_inputs);

    in_standby_l(in);
    pthread_mutex_destroy(&in->lock);
    pthread_mutex_destroy(&in->pre_lock);
    free(in->proc_buf_out);

#ifdef PREPROCESSING_ENABLED
 int i;

 for (i=0; i<in->num_preprocessors; i++) {
        free(in->preprocessors[i].channel_configs);
 }

 if (in->read_buf) {
        free(in->read_buf);
 }

 if (in->proc_buf_in) {
        free(in->proc_buf_in);
 }

 if (in->resampler) {
        release_resampler(in->resampler);
 }
#endif

    free(stream);

    pthread_mutex_unlock(&adev->lock_inputs);

 return;
}
