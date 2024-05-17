void IPCThreadState::freeBuffer(Parcel* parcel, const uint8_t* data,
 size_t  ,
 const binder_size_t*  ,
 size_t  , void*  )
{
    IF_LOG_COMMANDS() {
        alog << "Writing BC_FREE_BUFFER for " << data << endl;
 }
    ALOG_ASSERT(data != NULL, "Called with NULL data");
 if (parcel != NULL) parcel->closeFileDescriptors();
 IPCThreadState* state = self();
    state->mOut.writeInt32(BC_FREE_BUFFER);
    state->mOut.writePointer((uintptr_t)data);
}
