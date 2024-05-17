netdutils::Status XfrmController::ipSecAddSecurityAssociation(
 int32_t transformId, int32_t mode, const std::string& sourceAddress,
 const std::string& destinationAddress, int32_t underlyingNetId, int32_t spi, int32_t markValue,
 int32_t markMask, const std::string& authAlgo, const std::vector<uint8_t>& authKey,
 int32_t authTruncBits, const std::string& cryptAlgo, const std::vector<uint8_t>& cryptKey,
 int32_t cryptTruncBits, const std::string& aeadAlgo, const std::vector<uint8_t>& aeadKey,
 int32_t aeadIcvBits, int32_t encapType, int32_t encapLocalPort, int32_t encapRemotePort) {
    ALOGD("XfrmController::%s, line=%d", __FUNCTION__, __LINE__);
    ALOGD("transformId=%d", transformId);
    ALOGD("mode=%d", mode);
    ALOGD("sourceAddress=%s", sourceAddress.c_str());
    ALOGD("destinationAddress=%s", destinationAddress.c_str());
    ALOGD("underlyingNetworkId=%d", underlyingNetId);
    ALOGD("spi=%0.8x", spi);
    ALOGD("markValue=%x", markValue);
    ALOGD("markMask=%x", markMask);
    ALOGD("authAlgo=%s", authAlgo.c_str());
    ALOGD("authTruncBits=%d", authTruncBits);
    ALOGD("cryptAlgo=%s", cryptAlgo.c_str());
    ALOGD("cryptTruncBits=%d,", cryptTruncBits);
    ALOGD("aeadAlgo=%s", aeadAlgo.c_str());
    ALOGD("aeadIcvBits=%d,", aeadIcvBits);
    ALOGD("encapType=%d", encapType);
    ALOGD("encapLocalPort=%d", encapLocalPort);
    ALOGD("encapRemotePort=%d", encapRemotePort);

 XfrmSaInfo saInfo{};
    netdutils::Status ret = fillXfrmId(sourceAddress, destinationAddress, spi, markValue, markMask,
                                       transformId, &saInfo);
 if (!isOk(ret)) {
 return ret;
 }

    saInfo.auth = XfrmAlgo{
 .name = authAlgo, .key = authKey, .truncLenBits = static_cast<uint16_t>(authTruncBits)};

    saInfo.crypt = XfrmAlgo{
 .name = cryptAlgo, .key = cryptKey, .truncLenBits = static_cast<uint16_t>(cryptTruncBits)};

    saInfo.aead = XfrmAlgo{
 .name = aeadAlgo, .key = aeadKey, .truncLenBits = static_cast<uint16_t>(aeadIcvBits)};

 switch (static_cast<XfrmMode>(mode)) {
 case XfrmMode::TRANSPORT:
 case XfrmMode::TUNNEL:
            saInfo.mode = static_cast<XfrmMode>(mode);
 break;
 default:
 return netdutils::statusFromErrno(EINVAL, "Invalid xfrm mode");
 }

 XfrmSocketImpl sock;
    netdutils::Status socketStatus = sock.open();
 if (!isOk(socketStatus)) {
        ALOGD("Sock open failed for XFRM, line=%d", __LINE__);
 return socketStatus;
 }

 switch (static_cast<XfrmEncapType>(encapType)) {
 case XfrmEncapType::ESPINUDP:
 case XfrmEncapType::ESPINUDP_NON_IKE:
 if (saInfo.addrFamily != AF_INET) {
 return netdutils::statusFromErrno(EAFNOSUPPORT, "IPv6 encap not supported");
 }
            saInfo.encap.srcPort = encapLocalPort;
            saInfo.encap.dstPort = encapRemotePort;
 case XfrmEncapType::NONE:
            saInfo.encap.type = static_cast<XfrmEncapType>(encapType);
 break;
 default:
 return netdutils::statusFromErrno(EINVAL, "Invalid encap type");
 }

    saInfo.netId = underlyingNetId;

    ret = updateSecurityAssociation(saInfo, sock);
 if (!isOk(ret)) {
        ALOGD("Failed updating a Security Association, line=%d", __LINE__);
 }

 return ret;
}
