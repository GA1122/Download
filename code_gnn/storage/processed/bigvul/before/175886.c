netdutils::Status XfrmController::ipSecApplyTransportModeTransform(
 const android::base::unique_fd& socket, int32_t transformId, int32_t direction,
 const std::string& sourceAddress, const std::string& destinationAddress, int32_t spi) {
    ALOGD("XfrmController::%s, line=%d", __FUNCTION__, __LINE__);
    ALOGD("transformId=%d", transformId);
    ALOGD("direction=%d", direction);
    ALOGD("sourceAddress=%s", sourceAddress.c_str());
    ALOGD("destinationAddress=%s", destinationAddress.c_str());
    ALOGD("spi=%0.8x", spi);

 StatusOr<sockaddr_storage> ret = getSyscallInstance().getsockname<sockaddr_storage>(Fd(socket));
 if (!isOk(ret)) {
        ALOGE("Failed to get socket info in %s", __FUNCTION__);
 return ret;
 }
 struct sockaddr_storage saddr = ret.value();

 XfrmSaInfo saInfo{};
    netdutils::Status status =
        fillXfrmId(sourceAddress, destinationAddress, spi, 0, 0, transformId, &saInfo);
 if (!isOk(status)) {
        ALOGE("Couldn't build SA ID %s", __FUNCTION__);
 return status;
 }

 if (saddr.ss_family == AF_INET && saInfo.addrFamily != AF_INET) {
        ALOGE("IPV4 socket address family(%d) should match IPV4 Transform "
 "address family(%d)!",
              saddr.ss_family, saInfo.addrFamily);
 return netdutils::statusFromErrno(EINVAL, "Mismatched address family");
 }

 struct {
        xfrm_userpolicy_info info;
        xfrm_user_tmpl tmpl;
 } policy{};

    fillTransportModeUserSpInfo(saInfo, static_cast<XfrmDirection>(direction), &policy.info);
    fillUserTemplate(saInfo, &policy.tmpl);

    LOG_HEX("XfrmUserPolicy", reinterpret_cast<char*>(&policy), sizeof(policy));

 int sockOpt, sockLayer;
 switch (saddr.ss_family) {
 case AF_INET:
            sockOpt = IP_XFRM_POLICY;
            sockLayer = SOL_IP;
 break;
 case AF_INET6:
            sockOpt = IPV6_XFRM_POLICY;
            sockLayer = SOL_IPV6;
 break;
 default:
 return netdutils::statusFromErrno(EAFNOSUPPORT, "Invalid address family");
 }

    status = getSyscallInstance().setsockopt(Fd(socket), sockLayer, sockOpt, policy);
 if (!isOk(status)) {
        ALOGE("Error setting socket option for XFRM! (%s)", toString(status).c_str());
 }

 return status;
}
