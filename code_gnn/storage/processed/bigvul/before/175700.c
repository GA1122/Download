FLAC__StreamDecoderReadStatus FLACParser::read_callback(
 const FLAC__StreamDecoder *  , FLAC__byte buffer[],
 size_t *bytes, void *client_data)
{
 return ((FLACParser *) client_data)->readCallback(buffer, bytes);
}
