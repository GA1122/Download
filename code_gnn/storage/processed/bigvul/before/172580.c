android::SoftOMXComponent *createSoftOMXComponent(const char *name,
 const OMX_CALLBACKTYPE *callbacks, OMX_PTR appData,
        OMX_COMPONENTTYPE **component) {
    android::SoftHEVC *codec = new android::SoftHEVC(name, callbacks, appData, component);
 if (codec->init() != android::OK) {
        android::sp<android::SoftOMXComponent> release = codec;
 return NULL;
 }
 return codec;
}
