int XfrmController::removeVirtualTunnelInterface(const std::string& deviceName) {
    ALOGD("XfrmController::%s, line=%d", __FUNCTION__, __LINE__);
    ALOGD("deviceName=%s", deviceName.c_str());

 if (deviceName.empty()) {
 return EINVAL;
 }

 uint8_t PADDING_BUFFER[] = {0, 0, 0, 0};

    ifinfomsg ifInfoMsg{};
    nlattr iflaIfName;
 char iflaIfNameStrValue[deviceName.length() + 1];
 size_t iflaIfNameLength =
        strlcpy(iflaIfNameStrValue, deviceName.c_str(), sizeof(iflaIfNameStrValue));
 size_t iflaIfNamePad = fillNlAttr(IFLA_IFNAME, iflaIfNameLength, &iflaIfName);

    iovec iov[] = {
 {NULL, 0},
 {&ifInfoMsg, sizeof(ifInfoMsg)},

 {&iflaIfName, sizeof(iflaIfName)},
 {iflaIfNameStrValue, iflaIfNameLength},
 {&PADDING_BUFFER, iflaIfNamePad},
 };

 uint16_t action = RTM_DELLINK;
 uint16_t flags = NLM_F_REQUEST | NLM_F_ACK;

 int ret = -1 * sendNetlinkRequest(action, flags, iov, ARRAY_SIZE(iov), nullptr);
 if (ret) {
        ALOGE("Error in removing virtual tunnel interface %s. Error Code: %d", iflaIfNameStrValue,
              ret);
 }
 return ret;
}
