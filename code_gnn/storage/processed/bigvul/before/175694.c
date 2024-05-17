void FLACParser::metadata_callback(
 const FLAC__StreamDecoder *  ,
 const FLAC__StreamMetadata *metadata, void *client_data)
{
 ((FLACParser *) client_data)->metadataCallback(metadata);
}
