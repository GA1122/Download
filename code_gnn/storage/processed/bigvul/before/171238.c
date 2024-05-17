void SoundTriggerHwService::Module::binderDied(
 const wp<IBinder> &who __unused) {
    ALOGW("client binder died for module %d", mDescriptor.handle);
    detach();
}
