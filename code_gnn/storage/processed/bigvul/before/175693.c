void FLACParser::metadataCallback(const FLAC__StreamMetadata *metadata)
{
 switch (metadata->type) {
 case FLAC__METADATA_TYPE_STREAMINFO:
 if (!mStreamInfoValid) {
            mStreamInfo = metadata->data.stream_info;
            mStreamInfoValid = true;
 } else {
            ALOGE("FLACParser::metadataCallback unexpected STREAMINFO");
 }
 break;
 case FLAC__METADATA_TYPE_VORBIS_COMMENT:
 {
 const FLAC__StreamMetadata_VorbisComment *vc;
        vc = &metadata->data.vorbis_comment;
 for (FLAC__uint32 i = 0; i < vc->num_comments; ++i) {
            FLAC__StreamMetadata_VorbisComment_Entry *vce;
            vce = &vc->comments[i];
 if (mFileMetadata != 0 && vce->entry != NULL) {
                parseVorbisComment(mFileMetadata, (const char *) vce->entry,
                        vce->length);
 }
 }
 }
 break;
 case FLAC__METADATA_TYPE_PICTURE:
 if (mFileMetadata != 0) {
 const FLAC__StreamMetadata_Picture *p = &metadata->data.picture;
            mFileMetadata->setData(kKeyAlbumArt,
 MetaData::TYPE_NONE, p->data, p->data_length);
            mFileMetadata->setCString(kKeyAlbumArtMIME, p->mime_type);
 }
 break;
 default:
        ALOGW("FLACParser::metadataCallback unexpected type %u", metadata->type);
 break;
 }
}
