status_t CameraService::addListener(
 const sp<ICameraServiceListener>& listener) {
    ALOGV("%s: Add listener %p", __FUNCTION__, listener.get());

 if (listener == 0) {
        ALOGE("%s: Listener must not be null", __FUNCTION__);
 return BAD_VALUE;
 }

 Mutex::Autolock lock(mServiceLock);

 Vector<sp<ICameraServiceListener> >::iterator it, end;
 for (it = mListenerList.begin(); it != mListenerList.end(); ++it) {
 if ((*it)->asBinder() == listener->asBinder()) {
            ALOGW("%s: Tried to add listener %p which was already subscribed",
                  __FUNCTION__, listener.get());
 return ALREADY_EXISTS;
 }
 }

    mListenerList.push_back(listener);

  
 {
 Mutex::Autolock m(mStatusMutex) ;
 int numCams = getNumberOfCameras();
 for (int i = 0; i < numCams; ++i) {
            listener->onStatusChanged(mStatusList[i], i);
 }
 }

 return OK;
}
