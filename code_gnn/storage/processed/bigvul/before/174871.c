status_t CameraService::removeListener(
 const sp<ICameraServiceListener>& listener) {
    ALOGV("%s: Remove listener %p", __FUNCTION__, listener.get());

 if (listener == 0) {
        ALOGE("%s: Listener must not be null", __FUNCTION__);
 return BAD_VALUE;
 }

 Mutex::Autolock lock(mServiceLock);

 Vector<sp<ICameraServiceListener> >::iterator it;
 for (it = mListenerList.begin(); it != mListenerList.end(); ++it) {
 if ((*it)->asBinder() == listener->asBinder()) {
            mListenerList.erase(it);
 return OK;
 }
 }

    ALOGW("%s: Tried to remove a listener %p which was not subscribed",
          __FUNCTION__, listener.get());

 return BAD_VALUE;
}
