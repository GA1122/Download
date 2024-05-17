status_t OMXNodeInstance::freeNode(OMXMaster *master) {
    CLOG_LIFE(freeNode, "handle=%p", mHandle);
 static int32_t kMaxNumIterations = 10;

 if (mHandle == NULL) {
 return OK;
 }


    mDying = true;

    OMX_STATETYPE state;
    CHECK_EQ(OMX_GetState(mHandle, &state), OMX_ErrorNone);
 switch (state) {
 case OMX_StateExecuting:
 {
            ALOGV("forcing Executing->Idle");
            sendCommand(OMX_CommandStateSet, OMX_StateIdle);
            OMX_ERRORTYPE err;
 int32_t iteration = 0;
 while ((err = OMX_GetState(mHandle, &state)) == OMX_ErrorNone
 && state != OMX_StateIdle
 && state != OMX_StateInvalid) {
 if (++iteration > kMaxNumIterations) {
                    CLOGW("failed to enter Idle state (now %s(%d), aborting.",
                            asString(state), state);
                    state = OMX_StateInvalid;
 break;
 }

                usleep(100000);
 }
            CHECK_EQ(err, OMX_ErrorNone);

 if (state == OMX_StateInvalid) {
 break;
 }

 }

 case OMX_StateIdle:
 {
            ALOGV("forcing Idle->Loaded");
            sendCommand(OMX_CommandStateSet, OMX_StateLoaded);

            freeActiveBuffers();

            OMX_ERRORTYPE err;
 int32_t iteration = 0;
 while ((err = OMX_GetState(mHandle, &state)) == OMX_ErrorNone
 && state != OMX_StateLoaded
 && state != OMX_StateInvalid) {
 if (++iteration > kMaxNumIterations) {
                    CLOGW("failed to enter Loaded state (now %s(%d), aborting.",
                            asString(state), state);
                    state = OMX_StateInvalid;
 break;
 }

                ALOGV("waiting for Loaded state...");
                usleep(100000);
 }
            CHECK_EQ(err, OMX_ErrorNone);

 }

 case OMX_StateLoaded:
 case OMX_StateInvalid:
 break;

 default:
            LOG_ALWAYS_FATAL("unknown state %s(%#x).", asString(state), state);
 break;
 }

    ALOGV("[%x:%s] calling destroyComponentInstance", mNodeID, mName);
    OMX_ERRORTYPE err = master->destroyComponentInstance(
 static_cast<OMX_COMPONENTTYPE *>(mHandle));

    mHandle = NULL;
    CLOG_IF_ERROR(freeNode, err, "");
    free(mName);
    mName = NULL;

    mOwner->invalidateNodeID(mNodeID);
    mNodeID = 0;

    ALOGV("OMXNodeInstance going away.");
 delete this;

 return StatusFromOMXError(err);
}
