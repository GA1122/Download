 android::SoftOMXComponent *createSoftOMXComponent(
 const char *name, const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData, OMX_COMPONENTTYPE **component) {
 return new android::SoftAACEncoder(name, callbacks, appData, component);
}
