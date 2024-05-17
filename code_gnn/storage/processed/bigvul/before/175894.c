netdutils::Status XfrmController::processSecurityPolicy(int32_t transformId, int32_t direction,
 const std::string& localAddress,
 const std::string& remoteAddress,
 int32_t spi, int32_t markValue,
 int32_t markMask, int32_t msgType) {
    ALOGD("XfrmController::%s, line=%d", __FUNCTION__, __LINE__);
    ALOGD("transformId=%d", transformId);
    ALOGD("direction=%d", direction);
    ALOGD("localAddress=%s", localAddress.c_str());
    ALOGD("remoteAddress=%s", remoteAddress.c_str());
    ALOGD("spi=%0.8x", spi);
    ALOGD("markValue=%d", markValue);
    ALOGD("markMask=%d", markMask);
    ALOGD("msgType=%d", msgType);

 XfrmSaInfo saInfo{};
    saInfo.mode = XfrmMode::TUNNEL;

 XfrmSocketImpl sock;
    RETURN_IF_NOT_OK(sock.open());

    RETURN_IF_NOT_OK(
        fillXfrmId(localAddress, remoteAddress, spi, markValue, markMask, transformId, &saInfo));

 if (msgType == XFRM_MSG_DELPOLICY) {
 return deleteTunnelModeSecurityPolicy(saInfo, sock, static_cast<XfrmDirection>(direction));
 } else {
 return updateTunnelModeSecurityPolicy(saInfo, sock, static_cast<XfrmDirection>(direction),
                                              msgType);
 }
}
