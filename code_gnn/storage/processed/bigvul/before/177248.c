void ACodec::notifyOfRenderedFrames(bool dropIncomplete, FrameRenderTracker::Info *until) {
    sp<AMessage> msg = mNotify->dup();
    msg->setInt32("what", CodecBase::kWhatOutputFramesRendered);
    std::list<FrameRenderTracker::Info> done =
        mRenderTracker.checkFencesAndGetRenderedFrames(until, dropIncomplete);

 for (std::list<FrameRenderTracker::Info>::const_iterator it = done.cbegin();
            it != done.cend(); ++it) {
 ssize_t index = it->getIndex();
 if (index >= 0 && (size_t)index < mBuffers[kPortIndexOutput].size()) {
            mBuffers[kPortIndexOutput].editItemAt(index).mRenderInfo = NULL;
 } else if (index >= 0) {
            ALOGE("invalid index %zd in %zu", index, mBuffers[kPortIndexOutput].size());
 }
 }

 if (MediaCodec::CreateFramesRenderedMessage(done, msg)) {
        msg->post();
 }
}
