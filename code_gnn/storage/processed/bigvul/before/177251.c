void ACodec::LoadedState::onCreateInputSurface(
 const sp<AMessage> &  ) {
    ALOGV("onCreateInputSurface");

    sp<AMessage> notify = mCodec->mNotify->dup();
    notify->setInt32("what", CodecBase::kWhatInputSurfaceCreated);

    sp<IGraphicBufferProducer> bufferProducer;
 status_t err = mCodec->mOMX->createInputSurface(
            mCodec->mNode, kPortIndexInput, &bufferProducer, &mCodec->mInputMetadataType);

 if (err == OK) {
        err = setupInputSurface();
 }

 if (err == OK) {
        notify->setObject("input-surface",
 new BufferProducerWrapper(bufferProducer));
 } else {
        ALOGE("[%s] onCreateInputSurface returning error %d",
                mCodec->mComponentName.c_str(), err);
        notify->setInt32("err", err);
 }
    notify->post();
}
