bool ACodec::ExecutingState::onOMXFrameRendered(int64_t mediaTimeUs, nsecs_t systemNano) {
    mCodec->onFrameRendered(mediaTimeUs, systemNano);
 return true;
}
