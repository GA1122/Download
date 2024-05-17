static int get_seek_page_info(stb_vorbis *f, ProbedPage *z)
{
   uint8 header[27], lacing[255];
   int i,len;

   z->page_start = stb_vorbis_get_file_offset(f);

   getn(f, header, 27);
   if (header[0] != 'O' || header[1] != 'g' || header[2] != 'g' || header[3] != 'S')
      return 0;
   getn(f, lacing, header[26]);

   len = 0;
   for (i=0; i < header[26]; ++i)
      len += lacing[i];

   z->page_end = z->page_start + 27 + header[26] + len;

   z->last_decoded_sample = header[6] + (header[7] << 8) + (header[8] << 16) + (header[9] << 24);

   set_file_offset(f, z->page_start);
   return 1;
}
