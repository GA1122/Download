status_t IPCThreadState::talkWithDriver(bool doReceive)
{
 if (mProcess->mDriverFD <= 0) {
 return -EBADF;
 }
 
    binder_write_read bwr;
 
 const bool needRead = mIn.dataPosition() >= mIn.dataSize();
 
 const size_t outAvail = (!doReceive || needRead) ? mOut.dataSize() : 0;
 
    bwr.write_size = outAvail;
    bwr.write_buffer = (uintptr_t)mOut.data();

 if (doReceive && needRead) {
        bwr.read_size = mIn.dataCapacity();
        bwr.read_buffer = (uintptr_t)mIn.data();
 } else {
        bwr.read_size = 0;
        bwr.read_buffer = 0;
 }

    IF_LOG_COMMANDS() {
 TextOutput::Bundle _b(alog);
 if (outAvail != 0) {
            alog << "Sending commands to driver: " << indent;
 const void* cmds = (const void*)bwr.write_buffer;
 const void* end = ((const uint8_t*)cmds)+bwr.write_size;
            alog << HexDump(cmds, bwr.write_size) << endl;
 while (cmds < end) cmds = printCommand(alog, cmds);
            alog << dedent;
 }
        alog << "Size of receive buffer: " << bwr.read_size
 << ", needRead: " << needRead << ", doReceive: " << doReceive << endl;
 }
 
 if ((bwr.write_size == 0) && (bwr.read_size == 0)) return NO_ERROR;

    bwr.write_consumed = 0;
    bwr.read_consumed = 0;
 status_t err;
 do {
        IF_LOG_COMMANDS() {
            alog << "About to read/write, write size = " << mOut.dataSize() << endl;
 }
#if defined(HAVE_ANDROID_OS)
 if (ioctl(mProcess->mDriverFD, BINDER_WRITE_READ, &bwr) >= 0)
            err = NO_ERROR;
 else
            err = -errno;
#else
        err = INVALID_OPERATION;
#endif
 if (mProcess->mDriverFD <= 0) {
            err = -EBADF;
 }
        IF_LOG_COMMANDS() {
            alog << "Finished read/write, write size = " << mOut.dataSize() << endl;
 }
 } while (err == -EINTR);

    IF_LOG_COMMANDS() {
        alog << "Our err: " << (void*)(intptr_t)err << ", write consumed: "
 << bwr.write_consumed << " (of " << mOut.dataSize()
 << "), read consumed: " << bwr.read_consumed << endl;
 }

 if (err >= NO_ERROR) {
 if (bwr.write_consumed > 0) {
 if (bwr.write_consumed < mOut.dataSize())
                mOut.remove(0, bwr.write_consumed);
 else
                mOut.setDataSize(0);
 }
 if (bwr.read_consumed > 0) {
            mIn.setDataSize(bwr.read_consumed);
            mIn.setDataPosition(0);
 }
        IF_LOG_COMMANDS() {
 TextOutput::Bundle _b(alog);
            alog << "Remaining data size: " << mOut.dataSize() << endl;
            alog << "Received commands from driver: " << indent;
 const void* cmds = mIn.data();
 const void* end = mIn.data() + mIn.dataSize();
            alog << HexDump(cmds, mIn.dataSize()) << endl;
 while (cmds < end) cmds = printReturnCommand(alog, cmds);
            alog << dedent;
 }
 return NO_ERROR;
 }
 
 return err;
}
