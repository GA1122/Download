netdutils::Status XfrmController::ipSecAddSecurityPolicy(int32_t transformId, int32_t direction,
 const std::string& localAddress,
 const std::string& remoteAddress,
 int32_t spi, int32_t markValue,
 int32_t markMask) {
 return processSecurityPolicy(transformId, direction, localAddress, remoteAddress, spi,
                                 markValue, markMask, XFRM_MSG_NEWPOLICY);
}
