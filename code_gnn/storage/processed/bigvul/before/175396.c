void vorbis_book_clear(codebook *b){
  
 if(b->q_val)_ogg_free(b->q_val);
 if(b->dec_table)_ogg_free(b->dec_table);
 if(b->dec_buf)_ogg_free(b->dec_buf);

  memset(b,0,sizeof(*b));
}
