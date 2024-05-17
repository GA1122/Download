bool ACodec::UninitializedState::onMessageReceived(const sp<AMessage> &msg) {
 bool handled = false;

 switch (msg->what()) {
 case ACodec::kWhatSetup:
 {
            onSetup(msg);

            handled = true;
 break;
 }

 case ACodec::kWhatAllocateComponent:
 {
            onAllocateComponent(msg);
            handled = true;
 break;
 }

 case ACodec::kWhatShutdown:
 {
 int32_t keepComponentAllocated;
            CHECK(msg->findInt32(
 "keepComponentAllocated", &keepComponentAllocated));
            ALOGW_IF(keepComponentAllocated,
 "cannot keep component allocated on shutdown in Uninitialized state");

            sp<AMessage> notify = mCodec->mNotify->dup();
            notify->setInt32("what", CodecBase::kWhatShutdownCompleted);
            notify->post();

            handled = true;
 break;
 }

 case ACodec::kWhatFlush:
 {
            sp<AMessage> notify = mCodec->mNotify->dup();
            notify->setInt32("what", CodecBase::kWhatFlushCompleted);
            notify->post();

            handled = true;
 break;
 }

 case ACodec::kWhatReleaseCodecInstance:
 {
            handled = true;
 break;
 }

 default:
 return BaseState::onMessageReceived(msg);
 }

 return handled;
}
