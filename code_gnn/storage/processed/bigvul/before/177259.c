bool ACodec::LoadedToIdleState::onMessageReceived(const sp<AMessage> &msg) {
 switch (msg->what()) {
 case kWhatSetParameters:
 case kWhatShutdown:
 {
            mCodec->deferMessage(msg);
 return true;
 }

 case kWhatSignalEndOfInputStream:
 {
            mCodec->onSignalEndOfInputStream();
 return true;
 }

 case kWhatResume:
 {
 return true;
 }

 case kWhatFlush:
 {
            sp<AMessage> notify = mCodec->mNotify->dup();
            notify->setInt32("what", CodecBase::kWhatFlushCompleted);
            notify->post();
 return true;
 }

 default:
 return BaseState::onMessageReceived(msg);
 }
}
