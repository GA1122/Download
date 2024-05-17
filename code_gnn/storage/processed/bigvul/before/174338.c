int IPCThreadState::setupPolling(int* fd)
{
 if (mProcess->mDriverFD <= 0) {
 return -EBADF;
 }

    mOut.writeInt32(BC_ENTER_LOOPER);
 *fd = mProcess->mDriverFD;
 return 0;
}
