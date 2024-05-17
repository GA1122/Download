FLAC__StreamDecoderLengthStatus FLACParser::lengthCallback(
        FLAC__uint64 *stream_length)
{
 off64_t size;
 if (OK == mDataSource->getSize(&size)) {
 *stream_length = size;
 return FLAC__STREAM_DECODER_LENGTH_STATUS_OK;
 } else {
 return FLAC__STREAM_DECODER_LENGTH_STATUS_UNSUPPORTED;
 }
}
