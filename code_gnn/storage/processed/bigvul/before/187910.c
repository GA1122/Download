 FLAC__bool read_metadata_vorbiscomment_(FLAC__StreamDecoder *decoder, FLAC__StreamMetadata_VorbisComment *obj, unsigned length)
 {
 	FLAC__uint32 i;
 
 	FLAC__ASSERT(FLAC__bitreader_is_consumed_byte_aligned(decoder->private_->input));
 
   
  if (length >= 8) {
 		length -= 8;  
 		FLAC__ASSERT(FLAC__STREAM_METADATA_VORBIS_COMMENT_ENTRY_LENGTH_LEN == 32);
  if (!FLAC__bitreader_read_uint32_little_endian(decoder->private_->input, &obj->vendor_string.length))
  return false;  
  if (obj->vendor_string.length > 0) {
  if (length < obj->vendor_string.length) {
 				obj->vendor_string.length = 0;
 				obj->vendor_string.entry = 0;
  goto skip;
  }
  else
 				length -= obj->vendor_string.length;
  if (0 == (obj->vendor_string.entry = safe_malloc_add_2op_(obj->vendor_string.length,  1))) {
 				decoder->protected_->state = FLAC__STREAM_DECODER_MEMORY_ALLOCATION_ERROR;
  return false;
  }
  if (!FLAC__bitreader_read_byte_block_aligned_no_crc(decoder->private_->input, obj->vendor_string.entry, obj->vendor_string.length))
  return false;  
 			obj->vendor_string.entry[obj->vendor_string.length] = '\0';
  }
  else
 			obj->vendor_string.entry = 0;
 
   
 		FLAC__ASSERT(FLAC__STREAM_METADATA_VORBIS_COMMENT_NUM_COMMENTS_LEN == 32);
  if (!FLAC__bitreader_read_uint32_little_endian(decoder->private_->input, &obj->num_comments))
  return false;  
 
   
  if (obj->num_comments > 100000) {
   
 			obj->num_comments = 0;
  return false;
  }
 
  		if (obj->num_comments > 0) {
  			if (0 == (obj->comments = safe_malloc_mul_2op_p(obj->num_comments,  sizeof(FLAC__StreamMetadata_VorbisComment_Entry)))) {
  				decoder->protected_->state = FLAC__STREAM_DECODER_MEMORY_ALLOCATION_ERROR;
// 				obj->num_comments = 0;
  				return false;
  			}
  			for (i = 0; i < obj->num_comments; i++) {
   
 				obj->comments[i].length = 0;
 				obj->comments[i].entry = 0;
 
 				FLAC__ASSERT(FLAC__STREAM_METADATA_VORBIS_COMMENT_ENTRY_LENGTH_LEN == 32);
  if (length < 4) {
 					obj->num_comments = i;
  goto skip;
  }
  else
 					length -= 4;
  if (!FLAC__bitreader_read_uint32_little_endian(decoder->private_->input, &obj->comments[i].length))
  return false;  
  if (obj->comments[i].length > 0) {
  if (length < obj->comments[i].length) {
 						obj->num_comments = i;
  goto skip;
  }
  else
 						length -= obj->comments[i].length;
  if (0 == (obj->comments[i].entry = safe_malloc_add_2op_(obj->comments[i].length,  1))) {
 						decoder->protected_->state = FLAC__STREAM_DECODER_MEMORY_ALLOCATION_ERROR;
  return false;
  }
 					memset (obj->comments[i].entry, 0, obj->comments[i].length) ;
  if (!FLAC__bitreader_read_byte_block_aligned_no_crc(decoder->private_->input, obj->comments[i].entry, obj->comments[i].length)) {
 						obj->num_comments = i;
  goto skip;
  }
 					obj->comments[i].entry[obj->comments[i].length] = '\0';
  }
  else
 					obj->comments[i].entry = 0;
  }
  }
  else
 			obj->comments = 0;
  }
 
   skip:
  if (length > 0) {
   
  if(!FLAC__bitreader_skip_byte_block_aligned_no_crc(decoder->private_->input, length))
  return false;  
  }
 
  return true;
 }