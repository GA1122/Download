FLAC_API FLAC__bool FLAC__stream_decoder_seek_absolute(FLAC__StreamDecoder *decoder, FLAC__uint64 sample)
{
	FLAC__uint64 length;

	FLAC__ASSERT(0 != decoder);

 if(
		decoder->protected_->state != FLAC__STREAM_DECODER_SEARCH_FOR_METADATA &&
		decoder->protected_->state != FLAC__STREAM_DECODER_READ_METADATA &&
		decoder->protected_->state != FLAC__STREAM_DECODER_SEARCH_FOR_FRAME_SYNC &&
		decoder->protected_->state != FLAC__STREAM_DECODER_READ_FRAME &&
		decoder->protected_->state != FLAC__STREAM_DECODER_END_OF_STREAM
 )
 return false;

 if(0 == decoder->private_->seek_callback)
 return false;

	FLAC__ASSERT(decoder->private_->seek_callback);
	FLAC__ASSERT(decoder->private_->tell_callback);
	FLAC__ASSERT(decoder->private_->length_callback);
	FLAC__ASSERT(decoder->private_->eof_callback);

 if(FLAC__stream_decoder_get_total_samples(decoder) > 0 && sample >= FLAC__stream_decoder_get_total_samples(decoder))
 return false;

	decoder->private_->is_seeking = true;

  
	decoder->private_->do_md5_checking = false;

  
 if(decoder->private_->length_callback(decoder, &length, decoder->private_->client_data) != FLAC__STREAM_DECODER_LENGTH_STATUS_OK) {
		decoder->private_->is_seeking = false;
 return false;
 }

  
 if(
		decoder->protected_->state == FLAC__STREAM_DECODER_SEARCH_FOR_METADATA ||
		decoder->protected_->state == FLAC__STREAM_DECODER_READ_METADATA
 ) {
 if(!FLAC__stream_decoder_process_until_end_of_metadata(decoder)) {
  
			decoder->private_->is_seeking = false;
 return false;
 }
  
 if(FLAC__stream_decoder_get_total_samples(decoder) > 0 && sample >= FLAC__stream_decoder_get_total_samples(decoder)) {
			decoder->private_->is_seeking = false;
 return false;
 }
 }

 {
 const FLAC__bool ok =
#if FLAC__HAS_OGG
			decoder->private_->is_ogg?
			seek_to_absolute_sample_ogg_(decoder, length, sample) :
#endif
			seek_to_absolute_sample_(decoder, length, sample)
 ;
		decoder->private_->is_seeking = false;
 return ok;
 }
}
