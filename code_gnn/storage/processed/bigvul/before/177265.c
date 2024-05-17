bool ACodec::FlushingState::onMessageReceived(const sp<AMessage> &msg) {
 bool handled = false;

 switch (msg->what()) {
 case kWhatShutdown:
 {
            mCodec->deferMessage(msg);
 break;
 }

 case kWhatFlush:
 {
            handled = true;
 break;
 }

 default:
            handled = BaseState::onMessageReceived(msg);
 break;
 }

 return handled;
}
