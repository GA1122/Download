static jint android_net_wifi_start_sending_offloaded_packet(JNIEnv *env, jclass cls, jint iface,
                    jint idx, jbyteArray srcMac, jbyteArray dstMac, jbyteArray pkt, jint period) {
 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    ALOGD("Start packet offload [%d] = %p", idx, handle);
    wifi_error ret;
    wifi_request_id id = idx;
 
 ScopedBytesRO pktBytes(env, pkt), srcMacBytes(env, srcMac), dstMacBytes(env, dstMac);

    byte * pkt_data = (byte*) pktBytes.get();
 unsigned short pkt_len = env->GetArrayLength(pkt);
    byte* src_mac_addr = (byte*) srcMacBytes.get();
    byte* dst_mac_addr = (byte*) dstMacBytes.get();
 int i;
 char macAddr[32];
    sprintf(macAddr, "%0x:%0x:%0x:%0x:%0x:%0x", src_mac_addr[0], src_mac_addr[1],
            src_mac_addr[2], src_mac_addr[3], src_mac_addr[4], src_mac_addr[5]);
    ALOGD("src_mac_addr %s", macAddr);
    sprintf(macAddr, "%0x:%0x:%0x:%0x:%0x:%0x", dst_mac_addr[0], dst_mac_addr[1],
            dst_mac_addr[2], dst_mac_addr[3], dst_mac_addr[4], dst_mac_addr[5]);
    ALOGD("dst_mac_addr %s", macAddr);
    ALOGD("pkt_len %d\n", pkt_len);
    ALOGD("Pkt data : ");
 for(i = 0; i < pkt_len; i++) {
        ALOGD(" %x ", pkt_data[i]);
 }
    ALOGD("\n");
    ret =  hal_fn.wifi_start_sending_offloaded_packet(id, handle, pkt_data, pkt_len,
                src_mac_addr, dst_mac_addr, period);
    ALOGD("ret= %d\n", ret);
 return ret;
}
