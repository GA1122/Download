void CameraSourceListener::notify(int32_t msgType, int32_t ext1, int32_t ext2) {
    UNUSED_UNLESS_VERBOSE(msgType);
    UNUSED_UNLESS_VERBOSE(ext1);
    UNUSED_UNLESS_VERBOSE(ext2);
    ALOGV("notify(%d, %d, %d)", msgType, ext1, ext2);
}
