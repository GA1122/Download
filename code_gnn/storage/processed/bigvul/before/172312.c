static void on_ring_buffer_data(char *ring_name, char *buffer, int buffer_size,
        wifi_ring_buffer_status *status) {

 if (!ring_name || !buffer || !status ||
 (unsigned int)buffer_size <= sizeof(wifi_ring_buffer_entry)) {
        ALOGE("Error input for on_ring_buffer_data!");
 return;
 }


 JNIHelper helper(mVM);
  

 JNIObject<jobject> ringStatus = helper.createObject(
 "com/android/server/wifi/WifiNative$RingBufferStatus");
 if (status == NULL) {
        ALOGE("Error in creating ringBufferStatus");
 return;
 }

    helper.setStringField(ringStatus, "name", ring_name);
    helper.setIntField(ringStatus, "flag", status->flags);
    helper.setIntField(ringStatus, "ringBufferId", status->ring_id);
    helper.setIntField(ringStatus, "ringBufferByteSize", status->ring_buffer_byte_size);
    helper.setIntField(ringStatus, "verboseLevel", status->verbose_level);
    helper.setIntField(ringStatus, "writtenBytes", status->written_bytes);
    helper.setIntField(ringStatus, "readBytes", status->read_bytes);
    helper.setIntField(ringStatus, "writtenRecords", status->written_records);

 JNIObject<jbyteArray> bytes = helper.newByteArray(buffer_size);
    helper.setByteArrayRegion(bytes, 0, buffer_size, (jbyte*)buffer);

    helper.reportEvent(mCls,"onRingBufferData",
 "(Lcom/android/server/wifi/WifiNative$RingBufferStatus;[B)V",
            ringStatus.get(), bytes.get());
}
