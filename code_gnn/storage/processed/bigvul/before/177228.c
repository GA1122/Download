status_t ACodec::freeOutputBuffersNotOwnedByComponent() {
 status_t err = OK;
 for (size_t i = mBuffers[kPortIndexOutput].size(); i > 0;) {
        i--;
 BufferInfo *info =
 &mBuffers[kPortIndexOutput].editItemAt(i);

 if (info->mStatus != BufferInfo::OWNED_BY_COMPONENT &&
            info->mStatus != BufferInfo::OWNED_BY_DOWNSTREAM) {
 status_t err2 = freeBuffer(kPortIndexOutput, i);
 if (err == OK) {
                err = err2;
 }
 }
 }

 return err;
}
