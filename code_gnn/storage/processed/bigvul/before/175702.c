FLAC__StreamDecoderSeekStatus FLACParser::seekCallback(
        FLAC__uint64 absolute_byte_offset)
{
    mCurrentPos = absolute_byte_offset;
    mEOF = false;
 return FLAC__STREAM_DECODER_SEEK_STATUS_OK;
}
