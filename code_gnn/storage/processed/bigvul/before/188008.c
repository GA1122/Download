  long vorbis_book_decodev_add(codebook *book,ogg_int32_t *a,
			     oggpack_buffer *b,int n,int point){
//                              oggpack_buffer *b,int n,int point){
    if(book->used_entries>0){
      ogg_int32_t *v = book->dec_buf; 
      int i,j;
 
 
      if (!v) return -1;
      for(i=0;i<n;){
        if(decode_map(book,b,v,point))return -1;
      for (j=0;j<book->dim;j++)
	a[i++]+=v[j];
//       for (j=0;j<book->dim && i < n;j++)
//         a[i++]+=v[j];
      }
    }
    return 0;
  }