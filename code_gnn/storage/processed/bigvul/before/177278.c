bool ACodec::OutputPortSettingsChangedState::onOMXFrameRendered(
 int64_t mediaTimeUs, nsecs_t systemNano) {
    mCodec->onFrameRendered(mediaTimeUs, systemNano);
 return true;
}
