FLAC__bool frame_sync_(FLAC__StreamDecoder *decoder)
{
	FLAC__uint32 x;
	FLAC__bool first = true;

  
  
 if(FLAC__stream_decoder_get_total_samples(decoder) > 0) {
 if(decoder->private_->samples_decoded >= FLAC__stream_decoder_get_total_samples(decoder)) {
			decoder->protected_->state = FLAC__STREAM_DECODER_END_OF_STREAM;
 return true;
 }
 }

  
 if(!FLAC__bitreader_is_consumed_byte_aligned(decoder->private_->input)) {
 if(!FLAC__bitreader_read_raw_uint32(decoder->private_->input, &x, FLAC__bitreader_bits_left_for_byte_alignment(decoder->private_->input)))
 return false;  
 }

 while(1) {
 if(decoder->private_->cached) {
			x = (FLAC__uint32)decoder->private_->lookahead;
			decoder->private_->cached = false;
 }
 else {
 if(!FLAC__bitreader_read_raw_uint32(decoder->private_->input, &x, 8))
 return false;  
 }
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
 if(first) {
			send_error_to_client_(decoder, FLAC__STREAM_DECODER_ERROR_STATUS_LOST_SYNC);
			first = false;
 }
 }

 return true;
}
