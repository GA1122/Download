void Camera3Device::PreparerThread::setNotificationListener(NotificationListener *listener) {
 Mutex::Autolock l(mLock);
    mListener = listener;
}
