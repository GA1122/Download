void Camera3Device::setErrorStateV(const char *fmt, va_list args) {
 Mutex::Autolock l(mLock);
    setErrorStateLockedV(fmt, args);
}
