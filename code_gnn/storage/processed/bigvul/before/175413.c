static void adev_close_output_stream(struct audio_hw_device *dev,
 struct audio_stream_out *stream)
{
 struct stream_out *out = (struct stream_out *)stream;
 struct audio_device *adev = out->dev;
 (void)dev;

    ALOGV("%s: enter", __func__);
    out_standby(&stream->common);
    pthread_cond_destroy(&out->cond);
    pthread_mutex_destroy(&out->lock);
    pthread_mutex_destroy(&out->pre_lock);
    free(out->proc_buf_out);
    free(stream);
    ALOGV("%s: exit", __func__);
}
