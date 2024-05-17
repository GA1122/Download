FLAC__bool read_metadata_(FLAC__StreamDecoder *decoder)
{
	FLAC__bool is_last;
	FLAC__uint32 i, x, type, length;

	FLAC__ASSERT(FLAC__bitreader_is_consumed_byte_aligned(decoder->private_->input));

 if(!FLAC__bitreader_read_raw_uint32(decoder->private_->input, &x, FLAC__STREAM_METADATA_IS_LAST_LEN))
 return false;  
	is_last = x? true : false;

 if(!FLAC__bitreader_read_raw_uint32(decoder->private_->input, &type, FLAC__STREAM_METADATA_TYPE_LEN))
 return false;  

 if(!FLAC__bitreader_read_raw_uint32(decoder->private_->input, &length, FLAC__STREAM_METADATA_LENGTH_LEN))
 return false;  

 if(type == FLAC__METADATA_TYPE_STREAMINFO) {
 if(!read_metadata_streaminfo_(decoder, is_last, length))
 return false;

		decoder->private_->has_stream_info = true;
 if(0 == memcmp(decoder->private_->stream_info.data.stream_info.md5sum, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 16))
			decoder->private_->do_md5_checking = false;
 if(!decoder->private_->is_seeking && decoder->private_->metadata_filter[FLAC__METADATA_TYPE_STREAMINFO] && decoder->private_->metadata_callback)
			decoder->private_->metadata_callback(decoder, &decoder->private_->stream_info, decoder->private_->client_data);
 }
 else if(type == FLAC__METADATA_TYPE_SEEKTABLE) {
 if(!read_metadata_seektable_(decoder, is_last, length))
 return false;

		decoder->private_->has_seek_table = true;
 if(!decoder->private_->is_seeking && decoder->private_->metadata_filter[FLAC__METADATA_TYPE_SEEKTABLE] && decoder->private_->metadata_callback)
			decoder->private_->metadata_callback(decoder, &decoder->private_->seek_table, decoder->private_->client_data);
 }
 else {
		FLAC__bool skip_it = !decoder->private_->metadata_filter[type];
 unsigned real_length = length;
		FLAC__StreamMetadata block;

		memset(&block, 0, sizeof(block));
		block.is_last = is_last;
		block.type = (FLAC__MetadataType)type;
		block.length = length;

 if(type == FLAC__METADATA_TYPE_APPLICATION) {
 if(!FLAC__bitreader_read_byte_block_aligned_no_crc(decoder->private_->input, block.data.application.id, FLAC__STREAM_METADATA_APPLICATION_ID_LEN/8))
 return false;  

 if(real_length < FLAC__STREAM_METADATA_APPLICATION_ID_LEN/8) {  
				decoder->protected_->state = FLAC__STREAM_DECODER_MEMORY_ALLOCATION_ERROR; 
 return false;
 }

			real_length -= FLAC__STREAM_METADATA_APPLICATION_ID_LEN/8;

 if(decoder->private_->metadata_filter_ids_count > 0 && has_id_filtered_(decoder, block.data.application.id))
				skip_it = !skip_it;
 }

 if(skip_it) {
 if(!FLAC__bitreader_skip_byte_block_aligned_no_crc(decoder->private_->input, real_length))
 return false;  
 }
 else {
			FLAC__bool ok = true;
 switch(type) {
 case FLAC__METADATA_TYPE_PADDING:
  
 if(!FLAC__bitreader_skip_byte_block_aligned_no_crc(decoder->private_->input, real_length))
						ok = false;  
 break;
 case FLAC__METADATA_TYPE_APPLICATION:
  
 if(real_length > 0) {
 if(0 == (block.data.application.data = malloc(real_length))) {
							decoder->protected_->state = FLAC__STREAM_DECODER_MEMORY_ALLOCATION_ERROR;
							ok = false;
 }
 else if(!FLAC__bitreader_read_byte_block_aligned_no_crc(decoder->private_->input, block.data.application.data, real_length))
							ok = false;  
 }
 else
						block.data.application.data = 0;
 break;
 case FLAC__METADATA_TYPE_VORBIS_COMMENT:
 if(!read_metadata_vorbiscomment_(decoder, &block.data.vorbis_comment, real_length))
						ok = false;
 break;
 case FLAC__METADATA_TYPE_CUESHEET:
 if(!read_metadata_cuesheet_(decoder, &block.data.cue_sheet))
						ok = false;
 break;
 case FLAC__METADATA_TYPE_PICTURE:
 if(!read_metadata_picture_(decoder, &block.data.picture))
						ok = false;
 break;
 case FLAC__METADATA_TYPE_STREAMINFO:
 case FLAC__METADATA_TYPE_SEEKTABLE:
					FLAC__ASSERT(0);
 break;
 default:
 if(real_length > 0) {
 if(0 == (block.data.unknown.data = malloc(real_length))) {
							decoder->protected_->state = FLAC__STREAM_DECODER_MEMORY_ALLOCATION_ERROR;
							ok = false;
 }
 else if(!FLAC__bitreader_read_byte_block_aligned_no_crc(decoder->private_->input, block.data.unknown.data, real_length))
							ok = false;  
 }
 else
						block.data.unknown.data = 0;
 break;
 }
 if(ok && !decoder->private_->is_seeking && decoder->private_->metadata_callback)
				decoder->private_->metadata_callback(decoder, &block, decoder->private_->client_data);

  
 switch(type) {
 case FLAC__METADATA_TYPE_PADDING:
 break;
 case FLAC__METADATA_TYPE_APPLICATION:
 if(0 != block.data.application.data)
						free(block.data.application.data);
 break;
 case FLAC__METADATA_TYPE_VORBIS_COMMENT:
 if(0 != block.data.vorbis_comment.vendor_string.entry)
						free(block.data.vorbis_comment.vendor_string.entry);
 if(block.data.vorbis_comment.num_comments > 0)
 for(i = 0; i < block.data.vorbis_comment.num_comments; i++)
 if(0 != block.data.vorbis_comment.comments[i].entry)
								free(block.data.vorbis_comment.comments[i].entry);
 if(0 != block.data.vorbis_comment.comments)
						free(block.data.vorbis_comment.comments);
 break;
 case FLAC__METADATA_TYPE_CUESHEET:
 if(block.data.cue_sheet.num_tracks > 0)
 for(i = 0; i < block.data.cue_sheet.num_tracks; i++)
 if(0 != block.data.cue_sheet.tracks[i].indices)
								free(block.data.cue_sheet.tracks[i].indices);
 if(0 != block.data.cue_sheet.tracks)
						free(block.data.cue_sheet.tracks);
 break;
 case FLAC__METADATA_TYPE_PICTURE:
 if(0 != block.data.picture.mime_type)
						free(block.data.picture.mime_type);
 if(0 != block.data.picture.description)
						free(block.data.picture.description);
 if(0 != block.data.picture.data)
						free(block.data.picture.data);
 break;
 case FLAC__METADATA_TYPE_STREAMINFO:
 case FLAC__METADATA_TYPE_SEEKTABLE:
					FLAC__ASSERT(0);
 default:
 if(0 != block.data.unknown.data)
						free(block.data.unknown.data);
 break;
 }

 if(!ok)  
 return false;
 }
 }

 if(is_last) {
  
 if(!FLAC__stream_decoder_get_decode_position(decoder, &decoder->private_->first_frame_offset))
			decoder->private_->first_frame_offset = 0;
		decoder->protected_->state = FLAC__STREAM_DECODER_SEARCH_FOR_FRAME_SYNC;
 }

 return true;
}
