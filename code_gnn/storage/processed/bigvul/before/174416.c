FLAC__bool seek_to_absolute_sample_(FLAC__StreamDecoder *decoder, FLAC__uint64 stream_length, FLAC__uint64 target_sample)
{
	FLAC__uint64 first_frame_offset = decoder->private_->first_frame_offset, lower_bound, upper_bound, lower_bound_sample, upper_bound_sample, this_frame_sample;
	FLAC__int64 pos = -1;
 int i;
 unsigned approx_bytes_per_frame;
	FLAC__bool first_seek = true;
 const FLAC__uint64 total_samples = FLAC__stream_decoder_get_total_samples(decoder);
 const unsigned min_blocksize = decoder->private_->stream_info.data.stream_info.min_blocksize;
 const unsigned max_blocksize = decoder->private_->stream_info.data.stream_info.max_blocksize;
 const unsigned max_framesize = decoder->private_->stream_info.data.stream_info.max_framesize;
 const unsigned min_framesize = decoder->private_->stream_info.data.stream_info.min_framesize;
  
 unsigned channels = FLAC__stream_decoder_get_channels(decoder);
 unsigned bps = FLAC__stream_decoder_get_bits_per_sample(decoder);
 const FLAC__StreamMetadata_SeekTable *seek_table = decoder->private_->has_seek_table? &decoder->private_->seek_table.data.seek_table : 0;

  
 if(channels == 0)
		channels = decoder->private_->stream_info.data.stream_info.channels;
 if(bps == 0)
		bps = decoder->private_->stream_info.data.stream_info.bits_per_sample;

  
 if(max_framesize > 0)
		approx_bytes_per_frame = (max_framesize + min_framesize) / 2 + 1;
  
 else if(min_blocksize == max_blocksize && min_blocksize > 0) {
  
		approx_bytes_per_frame = min_blocksize * channels * bps/8 + 64;
 }
 else
		approx_bytes_per_frame = 4096 * channels * bps/8 + 64;

  
	lower_bound = first_frame_offset;
	lower_bound_sample = 0;
	upper_bound = stream_length;
	upper_bound_sample = total_samples > 0 ? total_samples : target_sample  ;

  
 if(seek_table) {
		FLAC__uint64 new_lower_bound = lower_bound;
		FLAC__uint64 new_upper_bound = upper_bound;
		FLAC__uint64 new_lower_bound_sample = lower_bound_sample;
		FLAC__uint64 new_upper_bound_sample = upper_bound_sample;

  
 for(i = (int)seek_table->num_points - 1; i >= 0; i--) {
 if(
				seek_table->points[i].sample_number != FLAC__STREAM_METADATA_SEEKPOINT_PLACEHOLDER &&
				seek_table->points[i].frame_samples > 0 &&  
 (total_samples <= 0 || seek_table->points[i].sample_number < total_samples) &&  
				seek_table->points[i].sample_number <= target_sample
 )
 break;
 }
 if(i >= 0) {  
			new_lower_bound = first_frame_offset + seek_table->points[i].stream_offset;
			new_lower_bound_sample = seek_table->points[i].sample_number;
 }

  
 for(i = 0; i < (int)seek_table->num_points; i++) {
 if(
				seek_table->points[i].sample_number != FLAC__STREAM_METADATA_SEEKPOINT_PLACEHOLDER &&
				seek_table->points[i].frame_samples > 0 &&  
 (total_samples <= 0 || seek_table->points[i].sample_number < total_samples) &&  
				seek_table->points[i].sample_number > target_sample
 )
 break;
 }
 if(i < (int)seek_table->num_points) {  
			new_upper_bound = first_frame_offset + seek_table->points[i].stream_offset;
			new_upper_bound_sample = seek_table->points[i].sample_number;
 }
  
 if(new_upper_bound >= new_lower_bound) {
			lower_bound = new_lower_bound;
			upper_bound = new_upper_bound;
			lower_bound_sample = new_lower_bound_sample;
			upper_bound_sample = new_upper_bound_sample;
 }
 }

	FLAC__ASSERT(upper_bound_sample >= lower_bound_sample);
  
 if(upper_bound_sample == lower_bound_sample)
		upper_bound_sample++;

	decoder->private_->target_sample = target_sample;
 while(1) {
  
 if (lower_bound_sample >= upper_bound_sample || lower_bound > upper_bound) {
			decoder->protected_->state = FLAC__STREAM_DECODER_SEEK_ERROR;
 return false;
 }
#ifndef FLAC__INTEGER_ONLY_LIBRARY
		pos = (FLAC__int64)lower_bound + (FLAC__int64)((FLAC__double)(target_sample - lower_bound_sample) / (FLAC__double)(upper_bound_sample - lower_bound_sample) * (FLAC__double)(upper_bound - lower_bound)) - approx_bytes_per_frame;
#else
  
 if(upper_bound - lower_bound < 0xffffffff)
			pos = (FLAC__int64)lower_bound + (FLAC__int64)(((target_sample - lower_bound_sample) * (upper_bound - lower_bound)) / (upper_bound_sample - lower_bound_sample)) - approx_bytes_per_frame;
 else  
			pos = (FLAC__int64)lower_bound + (FLAC__int64)((((target_sample - lower_bound_sample)>>8) * ((upper_bound - lower_bound)>>8)) / ((upper_bound_sample - lower_bound_sample)>>16)) - approx_bytes_per_frame;
#endif
 if(pos >= (FLAC__int64)upper_bound)
			pos = (FLAC__int64)upper_bound - 1;
 if(pos < (FLAC__int64)lower_bound)
			pos = (FLAC__int64)lower_bound;
 if(decoder->private_->seek_callback(decoder, (FLAC__uint64)pos, decoder->private_->client_data) != FLAC__STREAM_DECODER_SEEK_STATUS_OK) {
			decoder->protected_->state = FLAC__STREAM_DECODER_SEEK_ERROR;
 return false;
 }
 if(!FLAC__stream_decoder_flush(decoder)) {
  
 return false;
 }
  
		decoder->private_->unparseable_frame_count = 0;
 if(!FLAC__stream_decoder_process_single(decoder)) {
			decoder->protected_->state = FLAC__STREAM_DECODER_SEEK_ERROR;
 return false;
 }
  
  
#if 0
  
 if(decoder->protected_->state != FLAC__SEEKABLE_STREAM_DECODER_SEEKING && decoder->protected_->state != FLAC__STREAM_DECODER_END_OF_STREAM)
 break;
#endif
 if(!decoder->private_->is_seeking)
 break;

		FLAC__ASSERT(decoder->private_->last_frame.header.number_type == FLAC__FRAME_NUMBER_TYPE_SAMPLE_NUMBER);
		this_frame_sample = decoder->private_->last_frame.header.number.sample_number;

 if (0 == decoder->private_->samples_decoded || (this_frame_sample + decoder->private_->last_frame.header.blocksize >= upper_bound_sample && !first_seek)) {
 if (pos == (FLAC__int64)lower_bound) {
  
				decoder->protected_->state = FLAC__STREAM_DECODER_SEEK_ERROR;
 return false;
 }
  
			approx_bytes_per_frame = approx_bytes_per_frame? approx_bytes_per_frame * 2 : 16;
 continue;
 }
  
		first_seek = false;

  
 if (this_frame_sample < lower_bound_sample) {
			decoder->protected_->state = FLAC__STREAM_DECODER_SEEK_ERROR;
 return false;
 }

  
 if(target_sample < this_frame_sample) {
			upper_bound_sample = this_frame_sample + decoder->private_->last_frame.header.blocksize;
 
 if(!FLAC__stream_decoder_get_decode_position(decoder, &upper_bound)) {
				decoder->protected_->state = FLAC__STREAM_DECODER_SEEK_ERROR;
 return false;
 }
			approx_bytes_per_frame = (unsigned)(2 * (upper_bound - pos) / 3 + 16);
 }
 else {  
			lower_bound_sample = this_frame_sample + decoder->private_->last_frame.header.blocksize;
 if(!FLAC__stream_decoder_get_decode_position(decoder, &lower_bound)) {
				decoder->protected_->state = FLAC__STREAM_DECODER_SEEK_ERROR;
 return false;
 }
			approx_bytes_per_frame = (unsigned)(2 * (lower_bound - pos) / 3 + 16);
 }
 }

 return true;
}
