status_t OMX::freeNode(node_id node) {
 OMXNodeInstance *instance = findInstance(node);

 {
 Mutex::Autolock autoLock(mLock);
 ssize_t index = mLiveNodes.indexOfKey(IInterface::asBinder(instance->observer()));
 if (index < 0) {
 return OK;
 }
        mLiveNodes.removeItemsAt(index);
 }

 IInterface::asBinder(instance->observer())->unlinkToDeath(this);

 status_t err = instance->freeNode(mMaster);

 {
 Mutex::Autolock autoLock(mLock);
 ssize_t index = mDispatchers.indexOfKey(node);
        CHECK(index >= 0);
        mDispatchers.removeItemsAt(index);
 }

 return err;
}
