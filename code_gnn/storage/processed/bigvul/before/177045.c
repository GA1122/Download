void InputWindowHandle::releaseInfo() {
 if (mInfo) {
 delete mInfo;
        mInfo = NULL;
 }
}
