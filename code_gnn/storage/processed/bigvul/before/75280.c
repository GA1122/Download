static int is_whole_packet_present(stb_vorbis *f, int end_page)
{


   int s = f->next_seg, first = TRUE;
   uint8 *p = f->stream;

   if (s != -1) {  
      for (; s < f->segment_count; ++s) {
         p += f->segments[s];
         if (f->segments[s] < 255)                
            break;
      }
      if (end_page)
         if (s < f->segment_count-1)             return error(f, VORBIS_invalid_stream);
      if (s == f->segment_count)
         s = -1;  
      if (p > f->stream_end)                     return error(f, VORBIS_need_more_data);
      first = FALSE;
   }
   for (; s == -1;) {
      uint8 *q; 
      int n;

      if (p + 26 >= f->stream_end)               return error(f, VORBIS_need_more_data);
      if (memcmp(p, ogg_page_header, 4))         return error(f, VORBIS_invalid_stream);
      if (p[4] != 0)                             return error(f, VORBIS_invalid_stream);
      if (first) {  
         if (f->previous_length)
            if ((p[5] & PAGEFLAG_continued_packet))  return error(f, VORBIS_invalid_stream);
      } else {
         if (!(p[5] & PAGEFLAG_continued_packet)) return error(f, VORBIS_invalid_stream);
      }
      n = p[26];  
      q = p+27;   
      p = q + n;  
      if (p > f->stream_end)                     return error(f, VORBIS_need_more_data);
      for (s=0; s < n; ++s) {
         p += q[s];
         if (q[s] < 255)
            break;
      }
      if (end_page)
         if (s < n-1)                            return error(f, VORBIS_invalid_stream);
      if (s == n)
         s = -1;  
      if (p > f->stream_end)                     return error(f, VORBIS_need_more_data);
      first = FALSE;
   }
   return TRUE;
}
