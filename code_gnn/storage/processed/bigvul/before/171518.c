SimpleSoftOMXComponent::SimpleSoftOMXComponent(
 const char *name,
 const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component)
 : SoftOMXComponent(name, callbacks, appData, component),
      mLooper(new ALooper),
      mHandler(new AHandlerReflector<SimpleSoftOMXComponent>(this)),
      mState(OMX_StateLoaded),
      mTargetState(OMX_StateLoaded) {
    mLooper->setName(name);
    mLooper->registerHandler(mHandler);

    mLooper->start(
 false,  
 false,  
            ANDROID_PRIORITY_FOREGROUND);
}
