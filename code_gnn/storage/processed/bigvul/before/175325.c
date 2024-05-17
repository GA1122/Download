status_t MPEG4Extractor::parseColorInfo(off64_t offset, size_t size) {
 if (size < 4 || size == SIZE_MAX || mLastTrack == NULL) {
 return ERROR_MALFORMED;
 }

 uint8_t *buffer = new (std::nothrow) uint8_t[size + 1];
 if (buffer == NULL) {
 return ERROR_MALFORMED;
 }
 if (mDataSource->readAt(offset, buffer, size) != (ssize_t)size) {
 delete[] buffer;
        buffer = NULL;

 return ERROR_IO;
 }

 int32_t type = U32_AT(&buffer[0]);
 if ((type == FOURCC('n', 'c', 'l', 'x') && size >= 11)
 || (type == FOURCC('n', 'c', 'l', 'c') && size >= 10)) {
 int32_t primaries = U16_AT(&buffer[4]);
 int32_t transfer = U16_AT(&buffer[6]);
 int32_t coeffs = U16_AT(&buffer[8]);
 bool fullRange = (type == FOURCC('n', 'c', 'l', 'x')) && (buffer[10] & 128);

 ColorAspects aspects;
 ColorUtils::convertIsoColorAspectsToCodecAspects(
                primaries, transfer, coeffs, fullRange, aspects);

 if (!mLastTrack->meta->hasData(kKeyColorPrimaries)) {
            mLastTrack->meta->setInt32(kKeyColorPrimaries, aspects.mPrimaries);
            mLastTrack->meta->setInt32(kKeyTransferFunction, aspects.mTransfer);
            mLastTrack->meta->setInt32(kKeyColorMatrix, aspects.mMatrixCoeffs);
            mLastTrack->meta->setInt32(kKeyColorRange, aspects.mRange);
 }
 }

 delete[] buffer;
    buffer = NULL;

 return OK;
}
