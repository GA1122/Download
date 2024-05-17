static jboolean android_net_wifi_get_fw_memory_dump(JNIEnv *env, jclass cls, jint iface){

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);

 if (handle == NULL) {
        ALOGE("Can not get wifi_interface_handle");
 return false;
 }

    wifi_firmware_memory_dump_handler fw_dump_handle;
    fw_dump_handle.on_firmware_memory_dump = on_firmware_memory_dump;
 int result = hal_fn.wifi_get_firmware_memory_dump(handle, fw_dump_handle);
 return result == WIFI_SUCCESS;

}
