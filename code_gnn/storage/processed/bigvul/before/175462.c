static ssize_t in_read(struct audio_stream_in *stream, void *buffer,
 size_t bytes)
{
 struct stream_in *in = (struct stream_in *)stream;
 struct audio_device *adev = in->dev;
 ssize_t frames = -1;
 int ret = -1;
 int read_and_process_successful = false;

 size_t frames_rq = bytes / audio_stream_in_frame_size(stream);

  
    lock_input_stream(in);
 if (in->standby) {
        pthread_mutex_unlock(&in->lock);
        pthread_mutex_lock(&adev->lock_inputs);
        lock_input_stream(in);
 if (!in->standby) {
            pthread_mutex_unlock(&adev->lock_inputs);
 goto false_alarm;
 }
        pthread_mutex_lock(&adev->lock);
        ret = start_input_stream(in);
        pthread_mutex_unlock(&adev->lock);
        pthread_mutex_unlock(&adev->lock_inputs);
 if (ret != 0) {
 goto exit;
 }
        in->standby = 0;
 }
false_alarm:

 if (!list_empty(&in->pcm_dev_list)) {
 if (in->usecase == USECASE_AUDIO_CAPTURE_HOTWORD) {
            bytes = read_bytes_from_dsp(in, buffer, bytes);
 if (bytes > 0)
                read_and_process_successful = true;
 } else {
  
            frames = read_and_process_frames(stream, buffer, frames_rq);
 if (frames >= 0)
                read_and_process_successful = true;
 }
 }

  
 if (read_and_process_successful == true && adev->mic_mute)
        memset(buffer, 0, bytes);

exit:
    pthread_mutex_unlock(&in->lock);

 if (read_and_process_successful == false) {
        in_standby(&in->stream.common);
        ALOGV("%s: read failed - sleeping for buffer duration", __func__);
        usleep(bytes * 1000000 / audio_stream_in_frame_size(stream) /
               in->requested_rate);
 }
 return bytes;
}
