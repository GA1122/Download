FLAC__bool read_callback_(FLAC__byte buffer[], size_t *bytes, void *client_data)
{
	FLAC__StreamDecoder *decoder = (FLAC__StreamDecoder *)client_data;

 if(
#if FLAC__HAS_OGG
  
 !decoder->private_->is_ogg &&
#endif
		decoder->private_->eof_callback && decoder->private_->eof_callback(decoder, decoder->private_->client_data)
 ) {
 *bytes = 0;
		decoder->protected_->state = FLAC__STREAM_DECODER_END_OF_STREAM;
 return false;
 }
 else if(*bytes > 0) {
  
 if(decoder->private_->is_seeking && decoder->private_->unparseable_frame_count > 20) {
			decoder->protected_->state = FLAC__STREAM_DECODER_ABORTED;
 return false;
 }
 else {
 const FLAC__StreamDecoderReadStatus status =
#if FLAC__HAS_OGG
				decoder->private_->is_ogg?
				read_callback_ogg_aspect_(decoder, buffer, bytes) :
#endif
				decoder->private_->read_callback(decoder, buffer, bytes, decoder->private_->client_data)
 ;
 if(status == FLAC__STREAM_DECODER_READ_STATUS_ABORT) {
				decoder->protected_->state = FLAC__STREAM_DECODER_ABORTED;
 return false;
 }
 else if(*bytes == 0) {
 if(
					status == FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM ||
 (
#if FLAC__HAS_OGG
  
 !decoder->private_->is_ogg &&
#endif
						decoder->private_->eof_callback && decoder->private_->eof_callback(decoder, decoder->private_->client_data)
 )
 ) {
					decoder->protected_->state = FLAC__STREAM_DECODER_END_OF_STREAM;
 return false;
 }
 else
 return true;
 }
 else
 return true;
 }
 }
 else {
  
		decoder->protected_->state = FLAC__STREAM_DECODER_ABORTED;
 return false;
 }
  
}
