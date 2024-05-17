void FLACParser::error_callback(
 const FLAC__StreamDecoder *  ,
        FLAC__StreamDecoderErrorStatus status, void *client_data)
{
 ((FLACParser *) client_data)->errorCallback(status);
}
