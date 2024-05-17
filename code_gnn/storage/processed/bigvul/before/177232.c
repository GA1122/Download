void ACodec::BaseState::getMoreInputDataIfPossible() {
 if (mCodec->mPortEOS[kPortIndexInput]) {
 return;
 }

 BufferInfo *eligible = NULL;

 for (size_t i = 0; i < mCodec->mBuffers[kPortIndexInput].size(); ++i) {
 BufferInfo *info = &mCodec->mBuffers[kPortIndexInput].editItemAt(i);

#if 0
 if (info->mStatus == BufferInfo::OWNED_BY_UPSTREAM) {
 return;
 }
#endif

 if (info->mStatus == BufferInfo::OWNED_BY_US) {
            eligible = info;
 }
 }

 if (eligible == NULL) {
 return;
 }

    postFillThisBuffer(eligible);
}
