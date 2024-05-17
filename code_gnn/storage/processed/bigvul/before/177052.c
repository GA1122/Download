status_t SoftAVC::initDecoder() {
 if (H264SwDecInit(&mHandle, 0) == H264SWDEC_OK) {
 return OK;
 }
 return UNKNOWN_ERROR;
}
