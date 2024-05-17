status_t CameraClient::storeMetaDataInBuffers(bool enabled)
{
    LOG1("storeMetaDataInBuffers: %s", enabled? "true": "false");
 Mutex::Autolock lock(mLock);
 if (checkPidAndHardware() != NO_ERROR) {
 return UNKNOWN_ERROR;
 }
 return mHardware->storeMetaDataInBuffers(enabled);
}
