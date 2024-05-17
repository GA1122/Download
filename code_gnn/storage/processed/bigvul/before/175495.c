static ssize_t out_write(struct audio_stream_out *stream, const void *buffer,
 size_t bytes)
{
 struct stream_out *out = (struct stream_out *)stream;
 struct audio_device *adev = out->dev;
 ssize_t ret = 0;
 struct pcm_device *pcm_device;
 struct listnode *node;
 size_t frame_size = audio_stream_out_frame_size(stream);
 size_t frames_wr = 0, frames_rq = 0;
 unsigned char *data = NULL;
 struct pcm_config config;
#ifdef PREPROCESSING_ENABLED
 size_t in_frames = bytes / frame_size;
 size_t out_frames = in_frames;
 struct stream_in *in = NULL;
#endif

    lock_output_stream(out);
 if (out->standby) {
#ifdef PREPROCESSING_ENABLED
        pthread_mutex_unlock(&out->lock);
  
        pthread_mutex_lock(&adev->lock_inputs);
        lock_output_stream(out);
 if (!out->standby) {
            pthread_mutex_unlock(&adev->lock_inputs);
 goto false_alarm;
 }
#endif
        pthread_mutex_lock(&adev->lock);
        ret = start_output_stream(out);
 if (ret != 0) {
            pthread_mutex_unlock(&adev->lock);
#ifdef PREPROCESSING_ENABLED
            pthread_mutex_unlock(&adev->lock_inputs);
#endif
 goto exit;
 }
        out->standby = false;

#ifdef PREPROCESSING_ENABLED
  
 if (adev->active_input &&
 (adev->active_input->source == AUDIO_SOURCE_VOICE_COMMUNICATION ||
                adev->active_input->source == AUDIO_SOURCE_MIC)) {
                    in = adev->active_input;
                    ALOGV("%s: enter: force_input_standby true", __func__);
 }
#endif
        pthread_mutex_unlock(&adev->lock);
#ifdef PREPROCESSING_ENABLED
 if (!in) {
  
            pthread_mutex_unlock(&adev->lock_inputs);
 }
#endif
 }
false_alarm:

 if (out->muted)
        memset((void *)buffer, 0, bytes);
    list_for_each(node, &out->pcm_dev_list) {
        pcm_device = node_to_item(node, struct pcm_device, stream_list_node);
 if (pcm_device->resampler) {
 if (bytes * pcm_device->pcm_profile->config.rate / out->sample_rate + frame_size
 > pcm_device->res_byte_count) {
                pcm_device->res_byte_count =
                    bytes * pcm_device->pcm_profile->config.rate / out->sample_rate + frame_size;
                pcm_device->res_buffer =
                    realloc(pcm_device->res_buffer, pcm_device->res_byte_count);
                ALOGV("%s: resampler res_byte_count = %zu", __func__,
                    pcm_device->res_byte_count);
 }
            frames_rq = bytes / frame_size;
            frames_wr = pcm_device->res_byte_count / frame_size;
            ALOGVV("%s: resampler request frames = %zu frame_size = %zu",
                __func__, frames_rq, frame_size);
            pcm_device->resampler->resample_from_input(pcm_device->resampler,
 (int16_t *)buffer, &frames_rq, (int16_t *)pcm_device->res_buffer, &frames_wr);
            ALOGVV("%s: resampler output frames_= %zu", __func__, frames_wr);
 }
 if (pcm_device->pcm) {
 size_t src_channels = audio_channel_count_from_out_mask(out->channel_mask);
 size_t dst_channels = pcm_device->pcm_profile->config.channels;
 bool channel_remapping_needed = (dst_channels != src_channels);
 unsigned audio_bytes;
 const void *audio_data;

            ALOGVV("%s: writing buffer (%zd bytes) to pcm device", __func__, bytes);
 if (pcm_device->resampler && pcm_device->res_buffer) {
                audio_data = pcm_device->res_buffer;
                audio_bytes = frames_wr * frame_size;
 } else {
                audio_data = buffer;
                audio_bytes = bytes;
 }

  
            apply_dsp(pcm_device, audio_data, audio_bytes/4);

 if (channel_remapping_needed) {
 const void *remapped_audio_data;
 size_t dest_buffer_size = audio_bytes * dst_channels / src_channels;
 size_t new_size;
 size_t bytes_per_sample = audio_bytes_per_sample(stream->common.get_format(&stream->common));

  
 if (out->proc_buf_size < dest_buffer_size) {
                    out->proc_buf_size = dest_buffer_size;
                    out->proc_buf_out = realloc(out->proc_buf_out, dest_buffer_size);
                    ALOG_ASSERT((out->proc_buf_out != NULL),
 "out_write() failed to reallocate proc_buf_out");
 }
                new_size = adjust_channels(audio_data, src_channels, out->proc_buf_out, dst_channels,
                    bytes_per_sample, audio_bytes);
                ALOG_ASSERT(new_size == dest_buffer_size);
                audio_data = out->proc_buf_out;
                audio_bytes = dest_buffer_size;
 }

            pcm_device->status = pcm_write(pcm_device->pcm, audio_data, audio_bytes);
 if (pcm_device->status != 0)
                ret = pcm_device->status;
 }
 }
 if (ret == 0)
        out->written += bytes / frame_size;

exit:
    pthread_mutex_unlock(&out->lock);

 if (ret != 0) {
        list_for_each(node, &out->pcm_dev_list) {
            pcm_device = node_to_item(node, struct pcm_device, stream_list_node);
 if (pcm_device->pcm && pcm_device->status != 0)
                ALOGE("%s: error %zd - %s", __func__, ret, pcm_get_error(pcm_device->pcm));
 }
        out_standby(&out->stream.common);
        usleep(bytes * 1000000 / audio_stream_out_frame_size(stream) /
               out_get_sample_rate(&out->stream.common));
 }

#ifdef PREPROCESSING_ENABLED
 if (in) {
  
        lock_input_stream(in);
        pthread_mutex_lock(&adev->lock);
        LOG_ALWAYS_FATAL_IF(in != adev->active_input);
        do_in_standby_l(in);
        pthread_mutex_unlock(&adev->lock);
        pthread_mutex_unlock(&in->lock);
  
        pthread_mutex_unlock(&adev->lock_inputs);
 }
#endif

 return bytes;
}
