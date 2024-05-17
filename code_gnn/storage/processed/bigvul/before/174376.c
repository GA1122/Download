FLAC_API FLAC__bool FLAC__stream_decoder_reset(FLAC__StreamDecoder *decoder)
{
	FLAC__ASSERT(0 != decoder);
	FLAC__ASSERT(0 != decoder->private_);
	FLAC__ASSERT(0 != decoder->protected_);

 if(!FLAC__stream_decoder_flush(decoder)) {
  
 return false;
 }

#if FLAC__HAS_OGG
  
 if(decoder->private_->is_ogg)
		FLAC__ogg_decoder_aspect_reset(&decoder->protected_->ogg_decoder_aspect);
#endif

  
 if(!decoder->private_->internal_reset_hack) {
 if(decoder->private_->file == stdin)
 return false;  
 if(decoder->private_->seek_callback && decoder->private_->seek_callback(decoder, 0, decoder->private_->client_data) == FLAC__STREAM_DECODER_SEEK_STATUS_ERROR)
 return false;  
 }
 else
		decoder->private_->internal_reset_hack = false;

	decoder->protected_->state = FLAC__STREAM_DECODER_SEARCH_FOR_METADATA;

	decoder->private_->has_stream_info = false;
 if(decoder->private_->has_seek_table && 0 != decoder->private_->seek_table.data.seek_table.points) {
		free(decoder->private_->seek_table.data.seek_table.points);
		decoder->private_->seek_table.data.seek_table.points = 0;
		decoder->private_->has_seek_table = false;
 }
	decoder->private_->do_md5_checking = decoder->protected_->md5_checking;
  
	decoder->private_->fixed_block_size = decoder->private_->next_fixed_block_size = 0;

  
	FLAC__MD5Init(&decoder->private_->md5context);

	decoder->private_->first_frame_offset = 0;
	decoder->private_->unparseable_frame_count = 0;

 return true;
}