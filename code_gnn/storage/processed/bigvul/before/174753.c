status_t MPEG4Extractor::parseITunesMetaData(off64_t offset, size_t size) {
 if (size < 4 || size == SIZE_MAX) {
 return ERROR_MALFORMED;
 }

 uint8_t *buffer = new (std::nothrow) uint8_t[size + 1];
 if (buffer == NULL) {
 return ERROR_MALFORMED;
 }
 if (mDataSource->readAt(
                offset, buffer, size) != (ssize_t)size) {
 delete[] buffer;
        buffer = NULL;

 return ERROR_IO;
 }

 uint32_t flags = U32_AT(buffer);

 uint32_t metadataKey = 0;
 char chunk[5];
 MakeFourCCString(mPath[4], chunk);
    ALOGV("meta: %s @ %lld", chunk, offset);
 switch (mPath[4]) {
 case FOURCC(0xa9, 'a', 'l', 'b'):
 {
            metadataKey = kKeyAlbum;
 break;
 }
 case FOURCC(0xa9, 'A', 'R', 'T'):
 {
            metadataKey = kKeyArtist;
 break;
 }
 case FOURCC('a', 'A', 'R', 'T'):
 {
            metadataKey = kKeyAlbumArtist;
 break;
 }
 case FOURCC(0xa9, 'd', 'a', 'y'):
 {
            metadataKey = kKeyYear;
 break;
 }
 case FOURCC(0xa9, 'n', 'a', 'm'):
 {
            metadataKey = kKeyTitle;
 break;
 }
 case FOURCC(0xa9, 'w', 'r', 't'):
 {
            metadataKey = kKeyWriter;
 break;
 }
 case FOURCC('c', 'o', 'v', 'r'):
 {
            metadataKey = kKeyAlbumArt;
 break;
 }
 case FOURCC('g', 'n', 'r', 'e'):
 {
            metadataKey = kKeyGenre;
 break;
 }
 case FOURCC(0xa9, 'g', 'e', 'n'):
 {
            metadataKey = kKeyGenre;
 break;
 }
 case FOURCC('c', 'p', 'i', 'l'):
 {
 if (size == 9 && flags == 21) {
 char tmp[16];
                sprintf(tmp, "%d",
 (int)buffer[size - 1]);

                mFileMetaData->setCString(kKeyCompilation, tmp);
 }
 break;
 }
 case FOURCC('t', 'r', 'k', 'n'):
 {
 if (size == 16 && flags == 0) {
 char tmp[16];
 uint16_t* pTrack = (uint16_t*)&buffer[10];
 uint16_t* pTotalTracks = (uint16_t*)&buffer[12];
                sprintf(tmp, "%d/%d", ntohs(*pTrack), ntohs(*pTotalTracks));

                mFileMetaData->setCString(kKeyCDTrackNumber, tmp);
 }
 break;
 }
 case FOURCC('d', 'i', 's', 'k'):
 {
 if ((size == 14 || size == 16) && flags == 0) {
 char tmp[16];
 uint16_t* pDisc = (uint16_t*)&buffer[10];
 uint16_t* pTotalDiscs = (uint16_t*)&buffer[12];
                sprintf(tmp, "%d/%d", ntohs(*pDisc), ntohs(*pTotalDiscs));

                mFileMetaData->setCString(kKeyDiscNumber, tmp);
 }
 break;
 }
 case FOURCC('-', '-', '-', '-'):
 {
            buffer[size] = '\0';
 switch (mPath[5]) {
 case FOURCC('m', 'e', 'a', 'n'):
                    mLastCommentMean.setTo((const char *)buffer + 4);
 break;
 case FOURCC('n', 'a', 'm', 'e'):
                    mLastCommentName.setTo((const char *)buffer + 4);
 break;
 case FOURCC('d', 'a', 't', 'a'):
 if (size < 8) {
 delete[] buffer;
                        buffer = NULL;
                        ALOGE("b/24346430");
 return ERROR_MALFORMED;
 }
                    mLastCommentData.setTo((const char *)buffer + 8);
 break;
 }

 if ((mLastCommentMean.length() != 0) &&
 (mLastCommentName.length() != 0) &&
 (mLastCommentData.length() != 0)) {

 if (mLastCommentMean == "com.apple.iTunes"
 && mLastCommentName == "iTunSMPB") {
 int32_t delay, padding;
 if (sscanf(mLastCommentData,
 " %*x %x %x %*x", &delay, &padding) == 2) {
                        mLastTrack->meta->setInt32(kKeyEncoderDelay, delay);
                        mLastTrack->meta->setInt32(kKeyEncoderPadding, padding);
 }
 }

                mLastCommentMean.clear();
                mLastCommentName.clear();
                mLastCommentData.clear();
 }
 break;
 }

 default:
 break;
 }

 if (size >= 8 && metadataKey && !mFileMetaData->hasData(metadataKey)) {
 if (metadataKey == kKeyAlbumArt) {
            mFileMetaData->setData(
                    kKeyAlbumArt, MetaData::TYPE_NONE,
                    buffer + 8, size - 8);
 } else if (metadataKey == kKeyGenre) {
 if (flags == 0) {
 int genrecode = (int)buffer[size - 1];
                genrecode--;
 if (genrecode < 0) {
                    genrecode = 255;  
 }
 char genre[10];
                sprintf(genre, "%d", genrecode);

                mFileMetaData->setCString(metadataKey, genre);
 } else if (flags == 1) {
                buffer[size] = '\0';

                mFileMetaData->setCString(
                        metadataKey, (const char *)buffer + 8);
 }
 } else {
            buffer[size] = '\0';

            mFileMetaData->setCString(
                    metadataKey, (const char *)buffer + 8);
 }
 }

 delete[] buffer;
    buffer = NULL;

 return OK;
}
