void SoftAVC::saveFirstOutputBuffer(int32_t picId, uint8_t *data) {
    CHECK(mFirstPicture == NULL);
    mFirstPictureId = picId;

 uint32_t pictureSize = mWidth * mHeight * 3 / 2;
    mFirstPicture = new uint8_t[pictureSize];

     memcpy(mFirstPicture, data, pictureSize);
 }
