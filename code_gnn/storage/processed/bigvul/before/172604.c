size_t AudioFlinger::EffectModule::removeHandle(EffectHandle *handle)
{
 Mutex::Autolock _l(mLock);
 size_t size = mHandles.size();
 size_t i;
 for (i = 0; i < size; i++) {
 if (mHandles[i] == handle) {
 break;
 }
 }
 if (i == size) {
 return size;
 }
    ALOGV("removeHandle() %p removed handle %p in position %d", this, handle, i);

    mHandles.removeAt(i);
 if (i == 0) {
 EffectHandle *h = controlHandle_l();
 if (h != NULL) {
            h->setControl(true  , true   , handle->enabled()  );
 }
 }

 if (mHandles.size() == 0 && !mPinned) {
        mState = DESTROYED;
 }

 return mHandles.size();
}
