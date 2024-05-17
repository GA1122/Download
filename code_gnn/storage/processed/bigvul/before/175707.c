FLAC__StreamDecoderTellStatus FLACParser::tell_callback(
 const FLAC__StreamDecoder *  ,
        FLAC__uint64 *absolute_byte_offset, void *client_data)
{
 return ((FLACParser *) client_data)->tellCallback(absolute_byte_offset);
}
