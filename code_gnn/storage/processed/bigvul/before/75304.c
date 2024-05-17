int stb_vorbis_decode_frame_pushdata(
         stb_vorbis *f,                    
         const uint8 *data, int data_len,  
         int *channels,                    
         float ***output,                  
         int *samples                      
     )
{
   int i;
   int len,right,left;

   if (!IS_PUSH_MODE(f)) return error(f, VORBIS_invalid_api_mixing);

   if (f->page_crc_tests >= 0) {
      *samples = 0;
      return vorbis_search_for_page_pushdata(f, (uint8 *) data, data_len);
   }

   f->stream     = (uint8 *) data;
   f->stream_end = (uint8 *) data + data_len;
   f->error      = VORBIS__no_error;

   if (!is_whole_packet_present(f, FALSE)) {
      *samples = 0;
      return 0;
   }

   if (!vorbis_decode_packet(f, &len, &left, &right)) {
      enum STBVorbisError error = f->error;
      if (error == VORBIS_bad_packet_type) {
         f->error = VORBIS__no_error;
         while (get8_packet(f) != EOP)
            if (f->eof) break;
         *samples = 0;
         return (int) (f->stream - data);
      }
      if (error == VORBIS_continued_packet_flag_invalid) {
         if (f->previous_length == 0) {
            f->error = VORBIS__no_error;
            while (get8_packet(f) != EOP)
               if (f->eof) break;
            *samples = 0;
            return (int) (f->stream - data);
         }
      }
      stb_vorbis_flush_pushdata(f);
      f->error = error;
      *samples = 0;
      return 1;
   }

   len = vorbis_finish_frame(f, len, left, right);
   for (i=0; i < f->channels; ++i)
      f->outputs[i] = f->channel_buffers[i] + left;

   if (channels) *channels = f->channels;
   *samples = len;
   *output = f->outputs;
   return (int) (f->stream - data);
}
