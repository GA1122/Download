netdutils::Status XfrmController::ipSecDeleteSecurityAssociation(
 int32_t transformId, const std::string& sourceAddress, const std::string& destinationAddress,
 int32_t spi, int32_t markValue, int32_t markMask) {
    ALOGD("XfrmController:%s, line=%d", __FUNCTION__, __LINE__);
    ALOGD("transformId=%d", transformId);
    ALOGD("sourceAddress=%s", sourceAddress.c_str());
    ALOGD("destinationAddress=%s", destinationAddress.c_str());
    ALOGD("spi=%0.8x", spi);
    ALOGD("markValue=%x", markValue);
    ALOGD("markMask=%x", markMask);

 XfrmId saId{};
    netdutils::Status ret =
        fillXfrmId(sourceAddress, destinationAddress, spi, markValue, markMask, transformId, &saId);
 if (!isOk(ret)) {
 return ret;
 }

 XfrmSocketImpl sock;
    netdutils::Status socketStatus = sock.open();
 if (!isOk(socketStatus)) {
        ALOGD("Sock open failed for XFRM, line=%d", __LINE__);
 return socketStatus;
 }

    ret = deleteSecurityAssociation(saId, sock);
 if (!isOk(ret)) {
        ALOGD("Failed to delete Security Association, line=%d", __LINE__);
 }

 return ret;
}
