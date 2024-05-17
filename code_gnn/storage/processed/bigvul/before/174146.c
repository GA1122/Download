OMXNodeInstance *OMX::findInstance(node_id node) {
 Mutex::Autolock autoLock(mLock);

 ssize_t index = mNodeIDToInstance.indexOfKey(node);

 return index < 0 ? NULL : mNodeIDToInstance.valueAt(index);
}
