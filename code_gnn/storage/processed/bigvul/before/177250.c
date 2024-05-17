bool ACodec::LoadedState::onConfigureComponent(
 const sp<AMessage> &msg) {
    ALOGV("onConfigureComponent");

    CHECK(mCodec->mNode != 0);

 status_t err = OK;
 AString mime;
 if (!msg->findString("mime", &mime)) {
        err = BAD_VALUE;
 } else {
        err = mCodec->configureCodec(mime.c_str(), msg);
 }
 if (err != OK) {
        ALOGE("[%s] configureCodec returning error %d",
              mCodec->mComponentName.c_str(), err);

        mCodec->signalError(OMX_ErrorUndefined, makeNoSideEffectStatus(err));
 return false;
 }

 {
        sp<AMessage> notify = mCodec->mNotify->dup();
        notify->setInt32("what", CodecBase::kWhatComponentConfigured);
        notify->setMessage("input-format", mCodec->mInputFormat);
        notify->setMessage("output-format", mCodec->mOutputFormat);
        notify->post();
 }

 return true;
}
