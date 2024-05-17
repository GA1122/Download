bool AudioFlinger::EffectModule::purgeHandles()
{
 bool enabled = false;
 Mutex::Autolock _l(mLock);
 for (size_t i = 0; i < mHandles.size(); i++) {
 EffectHandle *handle = mHandles[i];
 if (handle != NULL && !handle->destroyed_l()) {
            handle->effect().clear();
 if (handle->hasControl()) {
                enabled = handle->enabled();
 }
 }
 }
 return enabled;
}
