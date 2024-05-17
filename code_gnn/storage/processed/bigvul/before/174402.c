FLAC__bool read_frame_(FLAC__StreamDecoder *decoder, FLAC__bool *got_a_frame, FLAC__bool do_full_decode)
{
 unsigned channel;
 unsigned i;
	FLAC__int32 mid, side;
 unsigned frame_crc;  
	FLAC__uint32 x;

 *got_a_frame = false;

  
	frame_crc = 0;
	frame_crc = FLAC__CRC16_UPDATE(decoder->private_->header_warmup[0], frame_crc);
	frame_crc = FLAC__CRC16_UPDATE(decoder->private_->header_warmup[1], frame_crc);
	FLAC__bitreader_reset_read_crc16(decoder->private_->input, (FLAC__uint16)frame_crc);

 if(!read_frame_header_(decoder))
 return false;
 if(decoder->protected_->state == FLAC__STREAM_DECODER_SEARCH_FOR_FRAME_SYNC)  
 return true;
 if(!allocate_output_(decoder, decoder->private_->frame.header.blocksize, decoder->private_->frame.header.channels))
 return false;
 for(channel = 0; channel < decoder->private_->frame.header.channels; channel++) {
  
 unsigned bps = decoder->private_->frame.header.bits_per_sample;
 switch(decoder->private_->frame.header.channel_assignment) {
 case FLAC__CHANNEL_ASSIGNMENT_INDEPENDENT:
  
 break;
 case FLAC__CHANNEL_ASSIGNMENT_LEFT_SIDE:
				FLAC__ASSERT(decoder->private_->frame.header.channels == 2);
 if(channel == 1)
					bps++;
 break;
 case FLAC__CHANNEL_ASSIGNMENT_RIGHT_SIDE:
				FLAC__ASSERT(decoder->private_->frame.header.channels == 2);
 if(channel == 0)
					bps++;
 break;
 case FLAC__CHANNEL_ASSIGNMENT_MID_SIDE:
				FLAC__ASSERT(decoder->private_->frame.header.channels == 2);
 if(channel == 1)
					bps++;
 break;
 default:
				FLAC__ASSERT(0);
 }
  
 if(!read_subframe_(decoder, channel, bps, do_full_decode))
 return false;
 if(decoder->protected_->state == FLAC__STREAM_DECODER_SEARCH_FOR_FRAME_SYNC)  
 return true;
 }
 if(!read_zero_padding_(decoder))
 return false;
 if(decoder->protected_->state == FLAC__STREAM_DECODER_SEARCH_FOR_FRAME_SYNC)  
 return true;

  
	frame_crc = FLAC__bitreader_get_read_crc16(decoder->private_->input);
 if(!FLAC__bitreader_read_raw_uint32(decoder->private_->input, &x, FLAC__FRAME_FOOTER_CRC_LEN))
 return false;  
 if(frame_crc == x) {
 if(do_full_decode) {
  
 switch(decoder->private_->frame.header.channel_assignment) {
 case FLAC__CHANNEL_ASSIGNMENT_INDEPENDENT:
  
 break;
 case FLAC__CHANNEL_ASSIGNMENT_LEFT_SIDE:
					FLAC__ASSERT(decoder->private_->frame.header.channels == 2);
 for(i = 0; i < decoder->private_->frame.header.blocksize; i++)
						decoder->private_->output[1][i] = decoder->private_->output[0][i] - decoder->private_->output[1][i];
 break;
 case FLAC__CHANNEL_ASSIGNMENT_RIGHT_SIDE:
					FLAC__ASSERT(decoder->private_->frame.header.channels == 2);
 for(i = 0; i < decoder->private_->frame.header.blocksize; i++)
						decoder->private_->output[0][i] += decoder->private_->output[1][i];
 break;
 case FLAC__CHANNEL_ASSIGNMENT_MID_SIDE:
					FLAC__ASSERT(decoder->private_->frame.header.channels == 2);
 for(i = 0; i < decoder->private_->frame.header.blocksize; i++) {
#if 1
						mid = decoder->private_->output[0][i];
						side = decoder->private_->output[1][i];
						mid <<= 1;
						mid |= (side & 1);  
						decoder->private_->output[0][i] = (mid + side) >> 1;
						decoder->private_->output[1][i] = (mid - side) >> 1;
#else
  
						mid = (decoder->private_->output[0][i] << 1) | (decoder->private_->output[1][i] & 1);  
						decoder->private_->output[0][i] = (mid + decoder->private_->output[1][i]) >> 1;
						decoder->private_->output[1][i] = (mid - decoder->private_->output[1][i]) >> 1;
#endif
 }
 break;
 default:
					FLAC__ASSERT(0);
 break;
 }
 }
 }
 else {
  
		send_error_to_client_(decoder, FLAC__STREAM_DECODER_ERROR_STATUS_FRAME_CRC_MISMATCH);
 if(do_full_decode) {
 for(channel = 0; channel < decoder->private_->frame.header.channels; channel++) {
				memset(decoder->private_->output[channel], 0, sizeof(FLAC__int32) * decoder->private_->frame.header.blocksize);
 }
 }
 }

 *got_a_frame = true;

  
 if(decoder->private_->next_fixed_block_size)
		decoder->private_->fixed_block_size = decoder->private_->next_fixed_block_size;

  
	decoder->protected_->channels = decoder->private_->frame.header.channels;
	decoder->protected_->channel_assignment = decoder->private_->frame.header.channel_assignment;
	decoder->protected_->bits_per_sample = decoder->private_->frame.header.bits_per_sample;
	decoder->protected_->sample_rate = decoder->private_->frame.header.sample_rate;
	decoder->protected_->blocksize = decoder->private_->frame.header.blocksize;

	FLAC__ASSERT(decoder->private_->frame.header.number_type == FLAC__FRAME_NUMBER_TYPE_SAMPLE_NUMBER);
	decoder->private_->samples_decoded = decoder->private_->frame.header.number.sample_number + decoder->private_->frame.header.blocksize;

  
 if(do_full_decode) {
 if(write_audio_frame_to_client_(decoder, &decoder->private_->frame, (const FLAC__int32 * const *)decoder->private_->output) != FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE)
 return false;
 }

	decoder->protected_->state = FLAC__STREAM_DECODER_SEARCH_FOR_FRAME_SYNC;
 return true;
}
