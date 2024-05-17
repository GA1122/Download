static ssize_t read_frames(struct stream_in *in, void *buffer, ssize_t frames)
{
 ssize_t frames_wr = 0;

 struct pcm_device *pcm_device;

 if (list_empty(&in->pcm_dev_list)) {
        ALOGE("%s: pcm device list empty", __func__);
 return -EINVAL;
 }

    pcm_device = node_to_item(list_head(&in->pcm_dev_list),
 struct pcm_device, stream_list_node);

 while (frames_wr < frames) {
 size_t frames_rd = frames - frames_wr;
        ALOGVV("%s: frames_rd: %zd, frames_wr: %zd, in->config.channels: %d",
               __func__,frames_rd,frames_wr,in->config.channels);
 if (in->resampler != NULL) {
            in->resampler->resample_from_provider(in->resampler,
 (int16_t *)((char *)buffer +
                            pcm_frames_to_bytes(pcm_device->pcm, frames_wr)),
 &frames_rd);
 } else {
 struct resampler_buffer buf = {
 { raw : NULL, },
                    frame_count : frames_rd,
 };
            get_next_buffer(&in->buf_provider, &buf);
 if (buf.raw != NULL) {
                memcpy((char *)buffer +
                            pcm_frames_to_bytes(pcm_device->pcm, frames_wr),
                        buf.raw,
                        pcm_frames_to_bytes(pcm_device->pcm, buf.frame_count));
                frames_rd = buf.frame_count;
 }
            release_buffer(&in->buf_provider, &buf);
 }
  
 if (in->read_status != 0)
 return in->read_status;

        frames_wr += frames_rd;
 }
 return frames_wr;
}
