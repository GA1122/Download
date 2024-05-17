bool ACodec::ExecutingState::onMessageReceived(const sp<AMessage> &msg) {
 bool handled = false;

 switch (msg->what()) {
 case kWhatShutdown:
 {
 int32_t keepComponentAllocated;
            CHECK(msg->findInt32(
 "keepComponentAllocated", &keepComponentAllocated));

            mCodec->mShutdownInProgress = true;
            mCodec->mExplicitShutdown = true;
            mCodec->mKeepComponentAllocated = keepComponentAllocated;

            mActive = false;

 status_t err = mCodec->mOMX->sendCommand(
                    mCodec->mNode, OMX_CommandStateSet, OMX_StateIdle);
 if (err != OK) {
 if (keepComponentAllocated) {
                    mCodec->signalError(OMX_ErrorUndefined, FAILED_TRANSACTION);
 }
 } else {
                mCodec->changeState(mCodec->mExecutingToIdleState);
 }

            handled = true;
 break;
 }

 case kWhatFlush:
 {
            ALOGV("[%s] ExecutingState flushing now "
 "(codec owns %zu/%zu input, %zu/%zu output).",
                    mCodec->mComponentName.c_str(),
                    mCodec->countBuffersOwnedByComponent(kPortIndexInput),
                    mCodec->mBuffers[kPortIndexInput].size(),
                    mCodec->countBuffersOwnedByComponent(kPortIndexOutput),
                    mCodec->mBuffers[kPortIndexOutput].size());

            mActive = false;

 status_t err = mCodec->mOMX->sendCommand(mCodec->mNode, OMX_CommandFlush, OMX_ALL);
 if (err != OK) {
                mCodec->signalError(OMX_ErrorUndefined, FAILED_TRANSACTION);
 } else {
                mCodec->changeState(mCodec->mFlushingState);
 }

            handled = true;
 break;
 }

 case kWhatResume:
 {
            resume();

            handled = true;
 break;
 }

 case kWhatRequestIDRFrame:
 {
 status_t err = mCodec->requestIDRFrame();
 if (err != OK) {
                ALOGW("Requesting an IDR frame failed.");
 }

            handled = true;
 break;
 }

 case kWhatSetParameters:
 {
            sp<AMessage> params;
            CHECK(msg->findMessage("params", &params));

 status_t err = mCodec->setParameters(params);

            sp<AMessage> reply;
 if (msg->findMessage("reply", &reply)) {
                reply->setInt32("err", err);
                reply->post();
 }

            handled = true;
 break;
 }

 case ACodec::kWhatSignalEndOfInputStream:
 {
            mCodec->onSignalEndOfInputStream();
            handled = true;
 break;
 }

 case kWhatSubmitOutputMetadataBufferIfEOS:
 {
 if (mCodec->mPortEOS[kPortIndexInput] &&
 !mCodec->mPortEOS[kPortIndexOutput]) {
 status_t err = mCodec->submitOutputMetadataBuffer();
 if (err == OK) {
                    mCodec->signalSubmitOutputMetadataBufferIfEOS_workaround();
 }
 }
 return true;
 }

 default:
            handled = BaseState::onMessageReceived(msg);
 break;
 }

 return handled;
}
