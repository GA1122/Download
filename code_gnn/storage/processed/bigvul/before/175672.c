bool SniffFLAC(
 const sp<DataSource> &source, String8 *mimeType, float *confidence,
        sp<AMessage> *)
{
 uint8_t header[4+4];
 if (source->readAt(0, header, sizeof(header)) != sizeof(header)
 || memcmp("fLaC\0\0\0\042", header, 4+4))
 {
 return false;
 }

 *mimeType = MEDIA_MIMETYPE_AUDIO_FLAC;
 *confidence = 0.5;

 return true;
}
