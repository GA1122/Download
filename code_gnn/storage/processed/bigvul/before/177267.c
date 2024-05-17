bool ACodec::BaseState::onOMXEmptyBufferDone(IOMX::buffer_id bufferID, int fenceFd) {
    ALOGV("[%s] onOMXEmptyBufferDone %u",
         mCodec->mComponentName.c_str(), bufferID);

 BufferInfo *info = mCodec->findBufferByID(kPortIndexInput, bufferID);
 BufferInfo::Status status = BufferInfo::getSafeStatus(info);
 if (status != BufferInfo::OWNED_BY_COMPONENT) {
        ALOGE("Wrong ownership in EBD: %s(%d) buffer #%u", _asString(status), status, bufferID);
        mCodec->dumpBuffers(kPortIndexInput);
 if (fenceFd >= 0) {
 ::close(fenceFd);
 }
 return false;
 }
    info->mStatus = BufferInfo::OWNED_BY_US;

 (void)mCodec->waitForFence(fenceFd, "onOMXEmptyBufferDone");
    fenceFd = -1;

    info->setWriteFence(fenceFd, "onOMXEmptyBufferDone");

    info->mData->setMediaBufferBase(NULL);

 PortMode mode = getPortMode(kPortIndexInput);

 switch (mode) {
 case KEEP_BUFFERS:
 break;

 case RESUBMIT_BUFFERS:
            postFillThisBuffer(info);
 break;

 case FREE_BUFFERS:
 default:
            ALOGE("SHOULD NOT REACH HERE: cannot free empty output buffers");
 return false;
 }

 return true;
}
