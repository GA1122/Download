FLAC__StreamDecoderWriteStatus FLACParser::write_callback(
 const FLAC__StreamDecoder *  , const FLAC__Frame *frame,
 const FLAC__int32 * const buffer[], void *client_data)
{
 return ((FLACParser *) client_data)->writeCallback(frame, buffer);
}
