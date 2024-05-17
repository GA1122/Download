CallbackThread::~CallbackThread() {
 if (mBuffer) {
        free(mBuffer);
        mBuffer = NULL;
 }
}
