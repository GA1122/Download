static int createSocketChannelNative(JNIEnv *env, jobject object, jint type,
                                     jstring name_str, jbyteArray uuidObj,
                                     jint channel, jint flag) {
 const char *service_name = NULL;
    jbyte *uuid = NULL;
 int socket_fd;
 bt_status_t status;

 if (!sBluetoothSocketInterface) return -1;

    ALOGV("%s: SOCK FLAG = %x", __FUNCTION__, flag);

 if(name_str != NULL) {
        service_name = env->GetStringUTFChars(name_str, NULL);
 }

 if(uuidObj != NULL) {
        uuid = env->GetByteArrayElements(uuidObj, NULL);
 if (!uuid) {
            ALOGE("failed to get uuid");
 goto Fail;
 }
 }
 if ( (status = sBluetoothSocketInterface->listen((btsock_type_t) type, service_name,
 (const uint8_t*) uuid, channel, &socket_fd, flag)) != BT_STATUS_SUCCESS) {
        ALOGE("Socket listen failed: %d", status);
 goto Fail;
 }

 if (socket_fd < 0) {
        ALOGE("Fail to creat file descriptor on socket fd");
 goto Fail;
 }
 if (service_name) env->ReleaseStringUTFChars(name_str, service_name);
 if (uuid) env->ReleaseByteArrayElements(uuidObj, uuid, 0);
 return socket_fd;

Fail:
 if (service_name) env->ReleaseStringUTFChars(name_str, service_name);
 if (uuid) env->ReleaseByteArrayElements(uuidObj, uuid, 0);
 return -1;
}
