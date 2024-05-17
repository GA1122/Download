int VisualizerLib_Create(const effect_uuid_t *uuid,
 int32_t  ,
 int32_t  ,
 effect_handle_t *pHandle) {
 int ret;
 int i;

 if (pHandle == NULL || uuid == NULL) {
 return -EINVAL;
 }

 if (memcmp(uuid, &gVisualizerDescriptor.uuid, sizeof(effect_uuid_t)) != 0) {
 return -EINVAL;
 }

 VisualizerContext *pContext = new VisualizerContext;

    pContext->mItfe = &gVisualizerInterface;
    pContext->mState = VISUALIZER_STATE_UNINITIALIZED;

    ret = Visualizer_init(pContext);
 if (ret < 0) {
        ALOGW("VisualizerLib_Create() init failed");
 delete pContext;
 return ret;
 }

 *pHandle = (effect_handle_t)pContext;

    pContext->mState = VISUALIZER_STATE_INITIALIZED;

    ALOGV("VisualizerLib_Create %p", pContext);

 return 0;

}
