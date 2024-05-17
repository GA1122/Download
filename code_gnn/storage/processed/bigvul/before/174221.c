int Camera3Device::RequestThread::getId(const wp<Camera3Device> &device) {
    sp<Camera3Device> d = device.promote();
 if (d != NULL) return d->mId;
 return 0;
}
