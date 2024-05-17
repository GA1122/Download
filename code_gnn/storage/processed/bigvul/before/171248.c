void SoundTriggerHwService::onFirstRef()
{
 const hw_module_t *mod;
 int rc;
    sound_trigger_hw_device *dev;

    rc = hw_get_module_by_class(SOUND_TRIGGER_HARDWARE_MODULE_ID, HW_MODULE_PREFIX, &mod);
 if (rc != 0) {
        ALOGE("couldn't load sound trigger module %s.%s (%s)",
              SOUND_TRIGGER_HARDWARE_MODULE_ID, HW_MODULE_PREFIX, strerror(-rc));
 return;
 }
    rc = sound_trigger_hw_device_open(mod, &dev);
 if (rc != 0) {
        ALOGE("couldn't open sound trigger hw device in %s.%s (%s)",
              SOUND_TRIGGER_HARDWARE_MODULE_ID, HW_MODULE_PREFIX, strerror(-rc));
 return;
 }
 if (dev->common.version < SOUND_TRIGGER_DEVICE_API_VERSION_1_0 ||
        dev->common.version > SOUND_TRIGGER_DEVICE_API_VERSION_CURRENT) {
        ALOGE("wrong sound trigger hw device version %04x", dev->common.version);
 return;
 }

    sound_trigger_module_descriptor descriptor;
    rc = dev->get_properties(dev, &descriptor.properties);
 if (rc != 0) {
        ALOGE("could not read implementation properties");
 return;
 }
    descriptor.handle =
 (sound_trigger_module_handle_t)android_atomic_inc(&mNextUniqueId);
    ALOGI("loaded default module %s, handle %d", descriptor.properties.description,
                                                 descriptor.handle);

    sp<ISoundTriggerClient> client;
    sp<Module> module = new Module(this, dev, descriptor, client);
    mModules.add(descriptor.handle, module);
    mCallbackThread = new CallbackThread(this);
}
