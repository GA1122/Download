FLAC__StreamDecoderReadStatus FLACParser::readCallback(
        FLAC__byte buffer[], size_t *bytes)
{
 size_t requested = *bytes;
 ssize_t actual = mDataSource->readAt(mCurrentPos, buffer, requested);
 if (0 > actual) {
 *bytes = 0;
 return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
 } else if (0 == actual) {
 *bytes = 0;
        mEOF = true;
 return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
 } else {
        assert(actual <= requested);
 *bytes = actual;
        mCurrentPos += actual;
 return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
 }
}
