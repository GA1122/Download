status_t IPCThreadState::writeTransactionData(int32_t cmd, uint32_t binderFlags,
 int32_t handle, uint32_t code, const Parcel& data, status_t* statusBuffer)
{
    binder_transaction_data tr;

    tr.target.ptr = 0;  
    tr.target.handle = handle;
    tr.code = code;
    tr.flags = binderFlags;
    tr.cookie = 0;
    tr.sender_pid = 0;
    tr.sender_euid = 0;
 
 const status_t err = data.errorCheck();
 if (err == NO_ERROR) {
        tr.data_size = data.ipcDataSize();
        tr.data.ptr.buffer = data.ipcData();
        tr.offsets_size = data.ipcObjectsCount()*sizeof(binder_size_t);
        tr.data.ptr.offsets = data.ipcObjects();
 } else if (statusBuffer) {
        tr.flags |= TF_STATUS_CODE;
 *statusBuffer = err;
        tr.data_size = sizeof(status_t);
        tr.data.ptr.buffer = reinterpret_cast<uintptr_t>(statusBuffer);
        tr.offsets_size = 0;
        tr.data.ptr.offsets = 0;
 } else {
 return (mLastError = err);
 }
 
    mOut.writeInt32(cmd);
    mOut.write(&tr, sizeof(tr));
 
 return NO_ERROR;
}
