bool ACodec::UninitializedState::onAllocateComponent(const sp<AMessage> &msg) {
    ALOGV("onAllocateComponent");

    CHECK(mCodec->mNode == 0);

 OMXClient client;
 if (client.connect() != OK) {
        mCodec->signalError(OMX_ErrorUndefined, NO_INIT);
 return false;
 }

    sp<IOMX> omx = client.interface();

    sp<AMessage> notify = new AMessage(kWhatOMXDied, mCodec);

    mDeathNotifier = new DeathNotifier(notify);
 if (IInterface::asBinder(omx)->linkToDeath(mDeathNotifier) != OK) {
        mDeathNotifier.clear();
 }

 Vector<OMXCodec::CodecNameAndQuirks> matchingCodecs;

 AString mime;

 AString componentName;
 uint32_t quirks = 0;
 int32_t encoder = false;
 if (msg->findString("componentName", &componentName)) {
 ssize_t index = matchingCodecs.add();
 OMXCodec::CodecNameAndQuirks *entry = &matchingCodecs.editItemAt(index);
        entry->mName = String8(componentName.c_str());

 if (!OMXCodec::findCodecQuirks(
                    componentName.c_str(), &entry->mQuirks)) {
            entry->mQuirks = 0;
 }
 } else {
        CHECK(msg->findString("mime", &mime));

 if (!msg->findInt32("encoder", &encoder)) {
            encoder = false;
 }

 OMXCodec::findMatchingCodecs(
                mime.c_str(),
                encoder,  
                NULL,  
 0,  
 &matchingCodecs);
 }

    sp<CodecObserver> observer = new CodecObserver;
    IOMX::node_id node = 0;

 status_t err = NAME_NOT_FOUND;
 for (size_t matchIndex = 0; matchIndex < matchingCodecs.size();
 ++matchIndex) {
        componentName = matchingCodecs.itemAt(matchIndex).mName.string();
        quirks = matchingCodecs.itemAt(matchIndex).mQuirks;

 pid_t tid = gettid();
 int prevPriority = androidGetThreadPriority(tid);
        androidSetThreadPriority(tid, ANDROID_PRIORITY_FOREGROUND);
        err = omx->allocateNode(componentName.c_str(), observer, &node);
        androidSetThreadPriority(tid, prevPriority);

 if (err == OK) {
 break;
 } else {
            ALOGW("Allocating component '%s' failed, try next one.", componentName.c_str());
 }

        node = 0;
 }

 if (node == 0) {
 if (!mime.empty()) {
            ALOGE("Unable to instantiate a %scoder for type '%s' with err %#x.",
                    encoder ? "en" : "de", mime.c_str(), err);
 } else {
            ALOGE("Unable to instantiate codec '%s' with err %#x.", componentName.c_str(), err);
 }

        mCodec->signalError((OMX_ERRORTYPE)err, makeNoSideEffectStatus(err));
 return false;
 }

    notify = new AMessage(kWhatOMXMessageList, mCodec);
    observer->setNotificationMessage(notify);

    mCodec->mComponentName = componentName;
    mCodec->mRenderTracker.setComponentName(componentName);
    mCodec->mFlags = 0;

 if (componentName.endsWith(".secure")) {
        mCodec->mFlags |= kFlagIsSecure;
        mCodec->mFlags |= kFlagIsGrallocUsageProtected;
        mCodec->mFlags |= kFlagPushBlankBuffersToNativeWindowOnShutdown;
 }

    mCodec->mQuirks = quirks;
    mCodec->mOMX = omx;
    mCodec->mNode = node;

 {
        sp<AMessage> notify = mCodec->mNotify->dup();
        notify->setInt32("what", CodecBase::kWhatComponentAllocated);
        notify->setString("componentName", mCodec->mComponentName.c_str());
        notify->post();
 }

    mCodec->changeState(mCodec->mLoadedState);

 return true;
}
