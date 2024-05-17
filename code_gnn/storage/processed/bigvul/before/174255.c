status_t Camera3Device::RequestThread::queueTriggerLocked(
 RequestTrigger trigger) {

 uint32_t tag = trigger.metadataTag;
 ssize_t index = mTriggerMap.indexOfKey(tag);

 switch (trigger.getTagType()) {
 case TYPE_BYTE:
 case TYPE_INT32:
 break;
 default:
            ALOGE("%s: Type not supported: 0x%x", __FUNCTION__,
                    trigger.getTagType());
 return INVALID_OPERATION;
 }

  
 if (index != NAME_NOT_FOUND) {
        mTriggerMap.editValueAt(index) = trigger;
 } else {
        mTriggerMap.add(tag, trigger);
 }

 return OK;
}
