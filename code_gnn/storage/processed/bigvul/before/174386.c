FLAC__bool allocate_output_(FLAC__StreamDecoder *decoder, unsigned size, unsigned channels)
{
 unsigned i;
	FLAC__int32 *tmp;

  
	size = size < FLAC__MAX_LPC_ORDER ? FLAC__MAX_LPC_ORDER : size ;

 if(size <= decoder->private_->output_capacity && channels <= decoder->private_->output_channels)
 return true;

  

 for(i = 0; i < FLAC__MAX_CHANNELS; i++) {
 if(0 != decoder->private_->output[i]) {
			free(decoder->private_->output[i]-4);
			decoder->private_->output[i] = 0;
 }
 if(0 != decoder->private_->residual_unaligned[i]) {
			free(decoder->private_->residual_unaligned[i]);
			decoder->private_->residual_unaligned[i] = decoder->private_->residual[i] = 0;
 }
 }

 for(i = 0; i < channels; i++) {
  
		tmp = safe_malloc_muladd2_(sizeof(FLAC__int32),  size,  4 );
 if(tmp == 0) {
			decoder->protected_->state = FLAC__STREAM_DECODER_MEMORY_ALLOCATION_ERROR;
 return false;
 }
		memset(tmp, 0, sizeof(FLAC__int32)*4);
		decoder->private_->output[i] = tmp + 4;

 if(!FLAC__memory_alloc_aligned_int32_array(size, &decoder->private_->residual_unaligned[i], &decoder->private_->residual[i])) {
			decoder->protected_->state = FLAC__STREAM_DECODER_MEMORY_ALLOCATION_ERROR;
 return false;
 }
 }

	decoder->private_->output_capacity = size;
	decoder->private_->output_channels = channels;

 return true;
}
