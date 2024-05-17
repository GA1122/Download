bool Camera3Device::get3AResult(const CameraMetadata& result, int32_t tag,
        T* value, uint32_t frameNumber) {
 (void) frameNumber;

 camera_metadata_ro_entry_t entry;

    entry = result.find(tag);
 if (entry.count == 0) {
        ALOGVV("%s: Camera %d: Frame %d: No %s provided by HAL!", __FUNCTION__,
            mId, frameNumber, get_camera_metadata_tag_name(tag));
 return false;
 }

 if (sizeof(T) == sizeof(uint8_t)) {
 *value = entry.data.u8[0];
 } else if (sizeof(T) == sizeof(int32_t)) {
 *value = entry.data.i32[0];
 } else {
        ALOGE("%s: Unexpected type", __FUNCTION__);
 return false;
 }
 return true;
}
