status_t AudioFlinger::EffectModule::addHandle(EffectHandle *handle)
{
 status_t status;

 Mutex::Autolock _l(mLock);
 int priority = handle->priority();
 size_t size = mHandles.size();
 EffectHandle *controlHandle = NULL;
 size_t i;
 for (i = 0; i < size; i++) {
 EffectHandle *h = mHandles[i];
 if (h == NULL || h->destroyed_l()) {
 continue;
 }
 if (controlHandle == NULL) {
            controlHandle = h;
 }
 if (h->priority() <= priority) {
 break;
 }
 }
 if (i == 0) {
 bool enabled = false;
 if (controlHandle != NULL) {
            enabled = controlHandle->enabled();
            controlHandle->setControl(false , true  , enabled  );
 }
        handle->setControl(true  , false  , enabled  );
        status = NO_ERROR;
 } else {
        status = ALREADY_EXISTS;
 }
    ALOGV("addHandle() %p added handle %p in position %d", this, handle, i);
    mHandles.insertAt(handle, i);
 return status;
}
