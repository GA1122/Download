bool ACodec::BaseState::onMessageReceived(const sp<AMessage> &msg) {
 switch (msg->what()) {
 case kWhatInputBufferFilled:
 {
            onInputBufferFilled(msg);
 break;
 }

 case kWhatOutputBufferDrained:
 {
            onOutputBufferDrained(msg);
 break;
 }

 case ACodec::kWhatOMXMessageList:
 {
 return checkOMXMessage(msg) ? onOMXMessageList(msg) : true;
 }

 case ACodec::kWhatOMXMessageItem:
 {
 return onOMXMessage(msg);
 }

 case ACodec::kWhatOMXMessage:
 {
 return checkOMXMessage(msg) ? onOMXMessage(msg) : true;
 }

 case ACodec::kWhatSetSurface:
 {
            sp<AReplyToken> replyID;
            CHECK(msg->senderAwaitsResponse(&replyID));

            sp<RefBase> obj;
            CHECK(msg->findObject("surface", &obj));

 status_t err = mCodec->handleSetSurface(static_cast<Surface *>(obj.get()));

            sp<AMessage> response = new AMessage;
            response->setInt32("err", err);
            response->postReply(replyID);
 break;
 }

 case ACodec::kWhatCreateInputSurface:
 case ACodec::kWhatSetInputSurface:
 case ACodec::kWhatSignalEndOfInputStream:
 {
            ALOGE("Message 0x%x was not handled", msg->what());
            mCodec->signalError(OMX_ErrorUndefined, INVALID_OPERATION);
 return true;
 }

 case ACodec::kWhatOMXDied:
 {
            ALOGE("OMX/mediaserver died, signalling error!");
            mCodec->signalError(OMX_ErrorResourcesLost, DEAD_OBJECT);
 break;
 }

 case ACodec::kWhatReleaseCodecInstance:
 {
            ALOGI("[%s] forcing the release of codec",
                    mCodec->mComponentName.c_str());
 status_t err = mCodec->mOMX->freeNode(mCodec->mNode);
            ALOGE_IF("[%s] failed to release codec instance: err=%d",
                       mCodec->mComponentName.c_str(), err);
            sp<AMessage> notify = mCodec->mNotify->dup();
            notify->setInt32("what", CodecBase::kWhatShutdownCompleted);
            notify->post();
 break;
 }

 default:
 return false;
 }

 return true;
}
