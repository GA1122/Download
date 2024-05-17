stb_vorbis *stb_vorbis_open_pushdata(
         const unsigned char *data, int data_len,  
         int *data_used,               
         int *error, const stb_vorbis_alloc *alloc)
{
   stb_vorbis *f, p;
   vorbis_init(&p, alloc);
   p.stream     = (uint8 *) data;
   p.stream_end = (uint8 *) data + data_len;
   p.push_mode  = TRUE;
   if (!start_decoder(&p)) {
      if (p.eof)
         *error = VORBIS_need_more_data;
      else
         *error = p.error;
      return NULL;
   }
   f = vorbis_alloc(&p);
   if (f) {
      *f = p;
      *data_used = (int) (f->stream - data);
      *error = 0;
      return f;
   } else {
      vorbis_deinit(&p);
      return NULL;
   }
}