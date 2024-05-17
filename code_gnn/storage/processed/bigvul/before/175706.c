FLAC__StreamDecoderTellStatus FLACParser::tellCallback(
        FLAC__uint64 *absolute_byte_offset)
{
 *absolute_byte_offset = mCurrentPos;
 return FLAC__STREAM_DECODER_TELL_STATUS_OK;
}
