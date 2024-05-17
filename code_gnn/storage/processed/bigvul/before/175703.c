FLAC__StreamDecoderSeekStatus FLACParser::seek_callback(
 const FLAC__StreamDecoder *  ,
        FLAC__uint64 absolute_byte_offset, void *client_data)
{
 return ((FLACParser *) client_data)->seekCallback(absolute_byte_offset);
}
