static int out_get_presentation_position(const struct audio_stream_out *stream,
 uint64_t *frames, struct timespec *timestamp)
{
 struct stream_out *out = (struct stream_out *)stream;
 int ret = -1;
 unsigned long dsp_frames;

    lock_output_stream(out);

  
 if (!list_empty(&out->pcm_dev_list)) {
 unsigned int avail;
 struct pcm_device *pcm_device = node_to_item(list_head(&out->pcm_dev_list),
 struct pcm_device, stream_list_node);

 if (pcm_get_htimestamp(pcm_device->pcm, &avail, timestamp) == 0) {
 size_t kernel_buffer_size = out->config.period_size * out->config.period_count;
 int64_t signed_frames = out->written - kernel_buffer_size + avail;
  
            signed_frames -=
 (render_latency(out->usecase) * out->sample_rate / 1000000LL);

  
 if (signed_frames >= 0) {
 *frames = signed_frames;
                ret = 0;
 }
 }
 }

    pthread_mutex_unlock(&out->lock);

 return ret;
}
