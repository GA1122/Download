FLAC__bool seek_to_absolute_sample_ogg_(FLAC__StreamDecoder *decoder, FLAC__uint64 stream_length, FLAC__uint64 target_sample)
{
	FLAC__uint64 left_pos = 0, right_pos = stream_length;
	FLAC__uint64 left_sample = 0, right_sample = FLAC__stream_decoder_get_total_samples(decoder);
	FLAC__uint64 this_frame_sample = (FLAC__uint64)0 - 1;
	FLAC__uint64 pos = 0;  
	FLAC__bool did_a_seek;
 unsigned iteration = 0;

  
 unsigned BINARY_SEARCH_AFTER_ITERATION = 2;

  
 static const FLAC__uint64 LINEAR_SEARCH_WITHIN_SAMPLES = FLAC__MAX_BLOCK_SIZE * 2;

  
 if(right_sample == 0) {
		right_sample = (FLAC__uint64)(-1);
		BINARY_SEARCH_AFTER_ITERATION = 0;
 }

	decoder->private_->target_sample = target_sample;
 for( ; ; iteration++) {
 if (iteration == 0 || this_frame_sample > target_sample || target_sample - this_frame_sample > LINEAR_SEARCH_WITHIN_SAMPLES) {
 if (iteration >= BINARY_SEARCH_AFTER_ITERATION) {
				pos = (right_pos + left_pos) / 2;
 }
 else {
#ifndef FLAC__INTEGER_ONLY_LIBRARY
				pos = (FLAC__uint64)((FLAC__double)(target_sample - left_sample) / (FLAC__double)(right_sample - left_sample) * (FLAC__double)(right_pos - left_pos));
#else
  
 if ((target_sample-left_sample <= 0xffffffff) && (right_pos-left_pos <= 0xffffffff))
					pos = (FLAC__int64)(((target_sample-left_sample) * (right_pos-left_pos)) / (right_sample-left_sample));
 else  
					pos = (FLAC__int64)((((target_sample-left_sample)>>8) * ((right_pos-left_pos)>>8)) / ((right_sample-left_sample)>>16));
#endif
  
 }

  
 if(decoder->private_->seek_callback((FLAC__StreamDecoder*)decoder, (FLAC__uint64)pos, decoder->private_->client_data) != FLAC__STREAM_DECODER_SEEK_STATUS_OK) {
				decoder->protected_->state = FLAC__STREAM_DECODER_SEEK_ERROR;
 return false;
 }
 if(!FLAC__stream_decoder_flush(decoder)) {
  
 return false;
 }
			did_a_seek = true;
 }
 else
			did_a_seek = false;

		decoder->private_->got_a_frame = false;
 if(!FLAC__stream_decoder_process_single(decoder)) {
			decoder->protected_->state = FLAC__STREAM_DECODER_SEEK_ERROR;
 return false;
 }
 if(!decoder->private_->got_a_frame) {
 if(did_a_seek) {
  
				right_pos = pos;
				BINARY_SEARCH_AFTER_ITERATION = 0;
 }
 else {
  
				decoder->protected_->state = FLAC__STREAM_DECODER_SEEK_ERROR;
 return false;
 }
 }
  
 else if(!decoder->private_->is_seeking) {
 break;
 }
 else {
			this_frame_sample = decoder->private_->last_frame.header.number.sample_number;
			FLAC__ASSERT(decoder->private_->last_frame.header.number_type == FLAC__FRAME_NUMBER_TYPE_SAMPLE_NUMBER);

 if (did_a_seek) {
 if (this_frame_sample <= target_sample) {
  
					FLAC__ASSERT(this_frame_sample != target_sample);

					left_sample = this_frame_sample;
  
 if (left_pos == pos) {
						decoder->protected_->state = FLAC__STREAM_DECODER_SEEK_ERROR;
 return false;
 }
					left_pos = pos;
 }
 else if(this_frame_sample > target_sample) {
					right_sample = this_frame_sample;
  
 if (right_pos == pos) {
						decoder->protected_->state = FLAC__STREAM_DECODER_SEEK_ERROR;
 return false;
 }
					right_pos = pos;
 }
 }
 }
 }

 return true;
}
