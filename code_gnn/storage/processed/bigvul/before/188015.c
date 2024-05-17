 static ssize_t read_and_process_frames(struct audio_stream_in *stream, void* buffer, ssize_t frames_num)
 {
  struct stream_in *in = (struct stream_in *)stream;
 
      ssize_t frames_wr = 0;  
      size_t bytes_per_sample = audio_bytes_per_sample(stream->common.get_format(&stream->common));
      void *proc_buf_out = buffer;
#ifdef PREPROCESSING_ENABLED
    audio_buffer_t in_buf;
    audio_buffer_t out_buf;
    int i;
    bool has_processing = in->num_preprocessors != 0;
#endif
// 
       
 
      size_t src_channels = in->config.channels;
      size_t dst_channels = audio_channel_count_from_in_mask(in->main_channels);
      bool channel_remapping_needed = (dst_channels != src_channels);
    size_t src_buffer_size = frames_num * src_channels * bytes_per_sample;
//     const size_t src_frame_size = src_channels * bytes_per_sample;
// 
// #ifdef PREPROCESSING_ENABLED
//     const bool has_processing = in->num_preprocessors != 0;
// #else
//     const bool has_processing = false;
// #endif
// 
//      
//     if (channel_remapping_needed || has_processing) {
//         const size_t src_buffer_size = frames_num * src_frame_size;
// 
//         if (in->proc_buf_size < src_buffer_size) {
//             in->proc_buf_size = src_buffer_size;
// #ifdef PREPROCESSING_ENABLED
//              
//             in->proc_buf_in = realloc(in->proc_buf_in, src_buffer_size);
//             ALOG_ASSERT((in->proc_buf_in != NULL),
//                     "process_frames() failed to reallocate proc_buf_in");
// #endif
//             in->proc_buf_out = realloc(in->proc_buf_out, src_buffer_size);
//             ALOG_ASSERT((in->proc_buf_out != NULL),
//                     "process_frames() failed to reallocate proc_buf_out");
//         }
//         if (channel_remapping_needed) {
//             proc_buf_out = in->proc_buf_out;
//         }
//     }
  
  #ifdef PREPROCESSING_ENABLED
      if (has_processing) {
   
 
          while (frames_wr < frames_num) {
               
              if (in->proc_buf_frames < (size_t)frames_num) {
                ssize_t frames_rd;
                if (in->proc_buf_size < (size_t)frames_num) {
                    in->proc_buf_size = (size_t)frames_num;
                    in->proc_buf_in = realloc(in->proc_buf_in, src_buffer_size);
                    ALOG_ASSERT((in->proc_buf_in != NULL),
                                "process_frames() failed to reallocate proc_buf_in");
                    if (channel_remapping_needed) {
                        in->proc_buf_out = realloc(in->proc_buf_out, src_buffer_size);
                        ALOG_ASSERT((in->proc_buf_out != NULL),
                                    "process_frames() failed to reallocate proc_buf_out");
                        proc_buf_out = in->proc_buf_out;
                    }
                }
                frames_rd = read_frames(in,
                                        in->proc_buf_in +
                                            in->proc_buf_frames * src_channels * bytes_per_sample,
                                        frames_num - in->proc_buf_frames);
                  if (frames_rd < 0) {
//                 ssize_t frames_rd = read_frames(in,
//                         (char *)in->proc_buf_in + in->proc_buf_frames * src_frame_size,
//                         frames_num - in->proc_buf_frames);
//                 if (frames_rd < 0) {
                       
                      frames_wr = frames_rd;
                      break;
  }
                 in->proc_buf_frames += frames_rd;
  }
 
  
                
//             audio_buffer_t in_buf;
//             audio_buffer_t out_buf;
// 
              in_buf.frameCount = in->proc_buf_frames;
            in_buf.s16 = in->proc_buf_in;
//             in_buf.s16 = in->proc_buf_in;   
              out_buf.frameCount = frames_num - frames_wr;
            out_buf.s16 = (int16_t *)proc_buf_out + frames_wr * in->config.channels;
//             out_buf.s16 = (int16_t *)proc_buf_out + frames_wr * src_channels;
  
               
            for (i = 0; i < in->num_preprocessors; i++) {
//             for (int i = 0; i < in->num_preprocessors; i++) {
                  (*in->preprocessors[i].effect_itfe)->process(in->preprocessors[i].effect_itfe,
                                                     &in_buf,
                                                     &out_buf);
  }
 
   
             in->proc_buf_frames -= in_buf.frameCount;
 
  
              if (in->proc_buf_frames) {
                  memcpy(in->proc_buf_in,
                       in->proc_buf_in + in_buf.frameCount * src_channels * bytes_per_sample,
                       in->proc_buf_frames * in->config.channels * audio_bytes_per_sample(in_get_format(in)));
//                        (char *)in->proc_buf_in + in_buf.frameCount * src_frame_size,
//                        in->proc_buf_frames * src_frame_size);
              }
  
               
  if (out_buf.frameCount == 0) {
                 ALOGW("No frames produced by preproc");
  continue;
  }
 
  if ((frames_wr + (ssize_t)out_buf.frameCount) <= frames_num) {
                 frames_wr += out_buf.frameCount;
  } else {
   
                 ALOGE("preprocessing produced too many frames: %d + %zd  > %d !",
  (unsigned int)frames_wr, out_buf.frameCount, (unsigned int)frames_num);
                 frames_wr = frames_num;
  }
  }
  }
  else
 
  #endif  
      {
           
        if (channel_remapping_needed) {
             
            if (in->proc_buf_size < src_buffer_size) {
                in->proc_buf_size = src_buffer_size;
                in->proc_buf_out = realloc(in->proc_buf_out, src_buffer_size);
                ALOG_ASSERT((in->proc_buf_out != NULL),
                            "process_frames() failed to reallocate proc_buf_out");
            }
            proc_buf_out = in->proc_buf_out;
        }
          frames_wr = read_frames(in, proc_buf_out, frames_num);
          ALOG_ASSERT(frames_wr <= frames_num, "read more frames than requested");
      }
  
    if (channel_remapping_needed) {
//      
//     if (channel_remapping_needed && frames_wr > 0) {
          size_t ret = adjust_channels(proc_buf_out, src_channels, buffer, dst_channels,
            bytes_per_sample, frames_wr * src_channels * bytes_per_sample);
//             bytes_per_sample, frames_wr * src_frame_size);
          ALOG_ASSERT(ret == (frames_wr * dst_channels * bytes_per_sample));
      }
  
  return frames_wr;
 }