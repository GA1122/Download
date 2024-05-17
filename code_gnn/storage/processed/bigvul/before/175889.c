XfrmController::ipSecRemoveTransportModeTransform(const android::base::unique_fd& socket) {
    ALOGD("XfrmController::%s, line=%d", __FUNCTION__, __LINE__);

 StatusOr<sockaddr_storage> ret = getSyscallInstance().getsockname<sockaddr_storage>(Fd(socket));
 if (!isOk(ret)) {
        ALOGE("Failed to get socket info in %s! (%s)", __FUNCTION__, toString(ret).c_str());
 return ret;
 }

 int sockOpt, sockLayer;
 switch (ret.value().ss_family) {
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

    netdutils::Status status =
        getSyscallInstance().setsockopt(Fd(socket), sockLayer, sockOpt, NULL, 0);
 if (!isOk(status)) {
        ALOGE("Error removing socket option for XFRM! (%s)", toString(status).c_str());
 }

 return status;
}
