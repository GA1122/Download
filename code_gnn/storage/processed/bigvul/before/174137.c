status_t OMX::allocateNode(
 const char *name, const sp<IOMXObserver> &observer, node_id *node) {
 Mutex::Autolock autoLock(mLock);

 *node = 0;

 OMXNodeInstance *instance = new OMXNodeInstance(this, observer, name);

    OMX_COMPONENTTYPE *handle;
    OMX_ERRORTYPE err = mMaster->makeComponentInstance(
            name, &OMXNodeInstance::kCallbacks,
            instance, &handle);

 if (err != OMX_ErrorNone) {
        ALOGE("FAILED to allocate omx component '%s' err=%s(%#x)", name, asString(err), err);

        instance->onGetHandleFailed();

 return StatusFromOMXError(err);
 }

 *node = makeNodeID(instance);
    mDispatchers.add(*node, new CallbackDispatcher(instance));

    instance->setHandle(*node, handle);

    mLiveNodes.add(IInterface::asBinder(observer), instance);
 IInterface::asBinder(observer)->linkToDeath(this);

 return OK;
}
