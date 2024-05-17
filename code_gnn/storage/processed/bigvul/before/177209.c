void ACodec::FlushingState::changeStateIfWeOwnAllBuffers() {
 if (mFlushComplete[kPortIndexInput]
 && mFlushComplete[kPortIndexOutput]
 && mCodec->allYourBuffersAreBelongToUs()) {
        mCodec->waitUntilAllPossibleNativeWindowBuffersAreReturnedToUs();

        mCodec->mRenderTracker.clear(systemTime(CLOCK_MONOTONIC));

        sp<AMessage> notify = mCodec->mNotify->dup();
        notify->setInt32("what", CodecBase::kWhatFlushCompleted);
        notify->post();

        mCodec->mPortEOS[kPortIndexInput] =
            mCodec->mPortEOS[kPortIndexOutput] = false;

        mCodec->mInputEOSResult = OK;

 if (mCodec->mSkipCutBuffer != NULL) {
            mCodec->mSkipCutBuffer->clear();
 }

        mCodec->changeState(mCodec->mExecutingState);
 }
}
