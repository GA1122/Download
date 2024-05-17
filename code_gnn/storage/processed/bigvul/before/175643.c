status_t SoftAACEncoder2::initEncoder() {
 if (AACENC_OK != aacEncOpen(&mAACEncoder, 0, 0)) {
        ALOGE("Failed to init AAC encoder");
 return UNKNOWN_ERROR;
 }
 return OK;
}
