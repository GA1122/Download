void ACodec::updateRenderInfoForDequeuedBuffer(
 ANativeWindowBuffer *buf, int fenceFd, BufferInfo *info) {

    info->mRenderInfo =
        mRenderTracker.updateInfoForDequeuedBuffer(
                buf, fenceFd, info - &mBuffers[kPortIndexOutput][0]);

    notifyOfRenderedFrames(false  , info->mRenderInfo);
}
