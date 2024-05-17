static int out_standby(struct audio_stream *stream)
{
 struct stream_out *out = (struct stream_out *)stream;
 struct audio_device *adev = out->dev;

    ALOGV("%s: enter: usecase(%d: %s)", __func__,
          out->usecase, use_case_table[out->usecase]);
    lock_output_stream(out);
 if (!out->standby) {
        pthread_mutex_lock(&adev->lock);
        do_out_standby_l(out);
        pthread_mutex_unlock(&adev->lock);
 }
    pthread_mutex_unlock(&out->lock);
    ALOGV("%s: exit", __func__);
 return 0;
}
