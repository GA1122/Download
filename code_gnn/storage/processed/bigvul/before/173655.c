status_t ATSParser::Stream::flush(SyncEvent *event) {
 if (mBuffer == NULL || mBuffer->size() == 0) {
 return OK;
 }

    ALOGV("flushing stream 0x%04x size = %zu", mElementaryPID, mBuffer->size());

 ABitReader br(mBuffer->data(), mBuffer->size());

 status_t err = parsePES(&br, event);

    mBuffer->setRange(0, 0);

 return err;
}
