void ACodec::LoadedState::onShutdown(bool keepComponentAllocated) {
 if (!keepComponentAllocated) {
 (void)mCodec->mOMX->freeNode(mCodec->mNode);

        mCodec->changeState(mCodec->mUninitializedState);
 }

 if (mCodec->mExplicitShutdown) {
        sp<AMessage> notify = mCodec->mNotify->dup();
        notify->setInt32("what", CodecBase::kWhatShutdownCompleted);
        notify->post();
        mCodec->mExplicitShutdown = false;
 }
}
