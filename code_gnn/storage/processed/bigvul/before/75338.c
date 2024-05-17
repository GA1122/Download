static int vorbis_search_for_page_pushdata(vorb *f, uint8 *data, int data_len)
{
   int i,n;
   for (i=0; i < f->page_crc_tests; ++i)
      f->scan[i].bytes_done = 0;

   if (f->page_crc_tests < STB_VORBIS_PUSHDATA_CRC_COUNT) {
      if (data_len < 4) return 0;
      data_len -= 3;  
      for (i=0; i < data_len; ++i) {
         if (data[i] == 0x4f) {
            if (0==memcmp(data+i, ogg_page_header, 4)) {
               int j,len;
               uint32 crc;
               if (i+26 >= data_len || i+27+data[i+26] >= data_len) {
                  data_len = i;
                  break;
               }
               len = 27 + data[i+26];
               for (j=0; j < data[i+26]; ++j)
                  len += data[i+27+j];
               crc = 0;
               for (j=0; j < 22; ++j)
                  crc = crc32_update(crc, data[i+j]);
               for (   ; j < 26; ++j)
                  crc = crc32_update(crc, 0);
               n = f->page_crc_tests++;
               f->scan[n].bytes_left = len-j;
               f->scan[n].crc_so_far = crc;
               f->scan[n].goal_crc = data[i+22] + (data[i+23] << 8) + (data[i+24]<<16) + (data[i+25]<<24);
               if (data[i+27+data[i+26]-1] == 255)
                  f->scan[n].sample_loc = ~0;
               else
                  f->scan[n].sample_loc = data[i+6] + (data[i+7] << 8) + (data[i+ 8]<<16) + (data[i+ 9]<<24);
               f->scan[n].bytes_done = i+j;
               if (f->page_crc_tests == STB_VORBIS_PUSHDATA_CRC_COUNT)
                  break;
            }
         }
      }
   }

   for (i=0; i < f->page_crc_tests;) {
      uint32 crc;
      int j;
      int n = f->scan[i].bytes_done;
      int m = f->scan[i].bytes_left;
      if (m > data_len - n) m = data_len - n;
      crc = f->scan[i].crc_so_far;
      for (j=0; j < m; ++j)
         crc = crc32_update(crc, data[n+j]);
      f->scan[i].bytes_left -= m;
      f->scan[i].crc_so_far = crc;
      if (f->scan[i].bytes_left == 0) {
         if (f->scan[i].crc_so_far == f->scan[i].goal_crc) {
            data_len = n+m;  
            f->page_crc_tests = -1;  
            f->previous_length = 0;  
            f->next_seg = -1;        
            f->current_loc = f->scan[i].sample_loc;  
            f->current_loc_valid = f->current_loc != ~0U;
            return data_len;
         }
         f->scan[i] = f->scan[--f->page_crc_tests];
      } else {
         ++i;
      }
   }

   return data_len;
}
