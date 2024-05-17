status_t IPCThreadState::transact(int32_t handle,
 uint32_t code, const Parcel& data,
 Parcel* reply, uint32_t flags)
{
 status_t err = data.errorCheck();

    flags |= TF_ACCEPT_FDS;

    IF_LOG_TRANSACTIONS() {
 TextOutput::Bundle _b(alog);
        alog << "BC_TRANSACTION thr " << (void*)pthread_self() << " / hand "
 << handle << " / code " << TypeCode(code) << ": "
 << indent << data << dedent << endl;
 }
 
 if (err == NO_ERROR) {
        LOG_ONEWAY(">>>> SEND from pid %d uid %d %s", getpid(), getuid(),
 (flags & TF_ONE_WAY) == 0 ? "READ REPLY" : "ONE WAY");
        err = writeTransactionData(BC_TRANSACTION, flags, handle, code, data, NULL);
 }
 
 if (err != NO_ERROR) {
 if (reply) reply->setError(err);
 return (mLastError = err);
 }
 
 if ((flags & TF_ONE_WAY) == 0) {
 #if 0
 if (code == 4) {  
            ALOGI(">>>>>> CALLING transaction 4");
 } else {
            ALOGI(">>>>>> CALLING transaction %d", code);
 }
 #endif
 if (reply) {
            err = waitForResponse(reply);
 } else {
 Parcel fakeReply;
            err = waitForResponse(&fakeReply);
 }
 #if 0
 if (code == 4) {  
            ALOGI("<<<<<< RETURNING transaction 4");
 } else {
            ALOGI("<<<<<< RETURNING transaction %d", code);
 }
 #endif
 
        IF_LOG_TRANSACTIONS() {
 TextOutput::Bundle _b(alog);
            alog << "BR_REPLY thr " << (void*)pthread_self() << " / hand "
 << handle << ": ";
 if (reply) alog << indent << *reply << dedent << endl;
 else alog << "(none requested)" << endl;
 }
 } else {
        err = waitForResponse(NULL, NULL);
 }
 
 return err;
}
