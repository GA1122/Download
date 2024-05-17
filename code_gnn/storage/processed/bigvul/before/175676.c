FLAC__bool FLACParser::eof_callback(
 const FLAC__StreamDecoder *  , void *client_data)
{
 return ((FLACParser *) client_data)->eofCallback();
}
