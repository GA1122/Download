long vorbis_book_decode(codebook *book, oggpack_buffer *b){
 if(book->dec_type)return -1;
 return decode_packed_entry_number(book,b);
}
