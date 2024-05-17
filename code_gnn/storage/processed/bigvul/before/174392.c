FLAC__bool find_metadata_(FLAC__StreamDecoder *decoder)
{
	FLAC__uint32 x;
 unsigned i, id;
	FLAC__bool first = true;

	FLAC__ASSERT(FLAC__bitreader_is_consumed_byte_aligned(decoder->private_->input));

 for(i = id = 0; i < 4; ) {
 if(decoder->private_->cached) {
			x = (FLAC__uint32)decoder->private_->lookahead;
			decoder->private_->cached = false;
 }
 else {
 if(!FLAC__bitreader_read_raw_uint32(decoder->private_->input, &x, 8))
 return false;  
 }
 if(x == FLAC__STREAM_SYNC_STRING[i]) {
			first = true;
			i++;
			id = 0;
 continue;
 }

 if(id >= 3)
 return false;

 if(x == ID3V2_TAG_[id]) {
			id++;
			i = 0;
 if(id == 3) {
 if(!skip_id3v2_tag_(decoder))
 return false;  
 }
 continue;
 }
		id = 0;
 if(x == 0xff) {  
			decoder->private_->header_warmup[0] = (FLAC__byte)x;
 if(!FLAC__bitreader_read_raw_uint32(decoder->private_->input, &x, 8))
 return false;  

  
  
 if(x == 0xff) {  
				decoder->private_->lookahead = (FLAC__byte)x;
				decoder->private_->cached = true;
 }
 else if(x >> 1 == 0x7c) {  
				decoder->private_->header_warmup[1] = (FLAC__byte)x;
				decoder->protected_->state = FLAC__STREAM_DECODER_READ_FRAME;
 return true;
 }
 }
		i = 0;
 if(first) {
			send_error_to_client_(decoder, FLAC__STREAM_DECODER_ERROR_STATUS_LOST_SYNC);
			first = false;
 }
 }

	decoder->protected_->state = FLAC__STREAM_DECODER_READ_METADATA;
 return true;
}
