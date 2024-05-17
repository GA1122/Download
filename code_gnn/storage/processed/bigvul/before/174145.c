sp<OMX::CallbackDispatcher> OMX::findDispatcher(node_id node) {
 Mutex::Autolock autoLock(mLock);

 ssize_t index = mDispatchers.indexOfKey(node);

 return index < 0 ? NULL : mDispatchers.valueAt(index);
}
