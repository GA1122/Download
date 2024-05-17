status_t Camera3Device::RequestThread::insertTriggers(
 const sp<CaptureRequest> &request) {

 Mutex::Autolock al(mTriggerMutex);

    sp<Camera3Device> parent = mParent.promote();
 if (parent == NULL) {
        CLOGE("RequestThread: Parent is gone");
 return DEAD_OBJECT;
 }

 CameraMetadata &metadata = request->mSettings;
 size_t count = mTriggerMap.size();

 for (size_t i = 0; i < count; ++i) {
 RequestTrigger trigger = mTriggerMap.valueAt(i);
 uint32_t tag = trigger.metadataTag;

 if (tag == ANDROID_CONTROL_AF_TRIGGER_ID || tag == ANDROID_CONTROL_AE_PRECAPTURE_ID) {
 bool isAeTrigger = (trigger.metadataTag == ANDROID_CONTROL_AE_PRECAPTURE_ID);
 uint32_t triggerId = static_cast<uint32_t>(trigger.entryValue);
 if (isAeTrigger) {
                request->mResultExtras.precaptureTriggerId = triggerId;
                mCurrentPreCaptureTriggerId = triggerId;
 } else {
                request->mResultExtras.afTriggerId = triggerId;
                mCurrentAfTriggerId = triggerId;
 }
 if (parent->mDeviceVersion >= CAMERA_DEVICE_API_VERSION_3_2) {
 continue;  
 }
 }

        camera_metadata_entry entry = metadata.find(tag);

 if (entry.count > 0) {
  
 RequestTrigger oldTrigger = trigger;

            oldTrigger.entryValue = entry.data.u8[0];

            mTriggerReplacedMap.add(tag, oldTrigger);
 } else {
  
            mTriggerRemovedMap.add(tag, trigger);
 }

 status_t res;

 switch (trigger.getTagType()) {
 case TYPE_BYTE: {
 uint8_t entryValue = static_cast<uint8_t>(trigger.entryValue);
                res = metadata.update(tag,
 &entryValue,
  1);
 break;
 }
 case TYPE_INT32:
                res = metadata.update(tag,
 &trigger.entryValue,
  1);
 break;
 default:
                ALOGE("%s: Type not supported: 0x%x",
                      __FUNCTION__,
                      trigger.getTagType());
 return INVALID_OPERATION;
 }

 if (res != OK) {
            ALOGE("%s: Failed to update request metadata with trigger tag %s"
 ", value %d", __FUNCTION__, trigger.getTagName(),
                  trigger.entryValue);
 return res;
 }

        ALOGV("%s: Mixed in trigger %s, value %d", __FUNCTION__,
              trigger.getTagName(),
              trigger.entryValue);
 }

    mTriggerMap.clear();

 return count;
}
