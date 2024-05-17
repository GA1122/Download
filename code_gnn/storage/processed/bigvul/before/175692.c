FLAC__StreamDecoderLengthStatus FLACParser::length_callback(
 const FLAC__StreamDecoder *  ,
        FLAC__uint64 *stream_length, void *client_data)
{
 return ((FLACParser *) client_data)->lengthCallback(stream_length);
}
