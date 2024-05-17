static media_status_t translate_error(status_t err) {
 if (err == OK) {
 return AMEDIA_OK;
 } else if (err == -EAGAIN) {
 return (media_status_t) AMEDIACODEC_INFO_TRY_AGAIN_LATER;
 }
    ALOGE("sf error code: %d", err);
 return AMEDIA_ERROR_UNKNOWN;
}
