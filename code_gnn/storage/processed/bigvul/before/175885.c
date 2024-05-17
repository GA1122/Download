netdutils::Status XfrmController::ipSecAllocateSpi(int32_t transformId,
 const std::string& sourceAddress,
 const std::string& destinationAddress,
 int32_t inSpi, int32_t* outSpi) {
    ALOGD("XfrmController:%s, line=%d", __FUNCTION__, __LINE__);
    ALOGD("transformId=%d", transformId);
    ALOGD("sourceAddress=%s", sourceAddress.c_str());
    ALOGD("destinationAddress=%s", destinationAddress.c_str());
    ALOGD("inSpi=%0.8x", inSpi);

 XfrmSaInfo saInfo{};
    netdutils::Status ret =
        fillXfrmId(sourceAddress, destinationAddress, INVALID_SPI, 0, 0, transformId, &saInfo);
 if (!isOk(ret)) {
 return ret;
 }

 XfrmSocketImpl sock;
    netdutils::Status socketStatus = sock.open();
 if (!isOk(socketStatus)) {
        ALOGD("Sock open failed for XFRM, line=%d", __LINE__);
 return socketStatus;
 }

 int minSpi = RAND_SPI_MIN, maxSpi = RAND_SPI_MAX;

 if (inSpi)
        minSpi = maxSpi = inSpi;

    ret = allocateSpi(saInfo, minSpi, maxSpi, reinterpret_cast<uint32_t*>(outSpi), sock);
 if (!isOk(ret)) {
        ALOGD("Failed to Allocate an SPI, line=%d", __LINE__);
 *outSpi = INVALID_SPI;
 }

 return ret;
}
