static void vorbis_init(stb_vorbis *p, const stb_vorbis_alloc *z)
{
   memset(p, 0, sizeof(*p));  
   if (z) {
      p->alloc = *z;
      p->alloc.alloc_buffer_length_in_bytes = (p->alloc.alloc_buffer_length_in_bytes+3) & ~3;
      p->temp_offset = p->alloc.alloc_buffer_length_in_bytes;
   }
   p->eof = 0;
   p->error = VORBIS__no_error;
   p->stream = NULL;
   p->codebooks = NULL;
   p->page_crc_tests = -1;
   #ifndef STB_VORBIS_NO_STDIO
   p->close_on_free = FALSE;
   p->f = NULL;
   #endif
}