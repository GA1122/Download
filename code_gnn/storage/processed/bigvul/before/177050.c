android::SoftOMXComponent *createSoftOMXComponent(
 const char *name, const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData, OMX_COMPONENTTYPE **component) {
 return new android::SoftAVC(name, callbacks, appData, component);
}