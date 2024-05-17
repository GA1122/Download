status_t AudioFlinger::EffectHandle::command(uint32_t cmdCode,
 uint32_t cmdSize,
 void *pCmdData,
 uint32_t *replySize,
 void *pReplyData)
{
    ALOGVV("command(), cmdCode: %d, mHasControl: %d, mEffect: %p",
            cmdCode, mHasControl, (mEffect == 0) ? 0 : mEffect.get());

 if (!mHasControl && cmdCode != EFFECT_CMD_GET_PARAM) {
 return INVALID_OPERATION;
 }
 if (mEffect == 0) {
 return DEAD_OBJECT;
 }
 if (mClient == 0) {
 return INVALID_OPERATION;
 }

 if (cmdCode == EFFECT_CMD_SET_PARAM_COMMIT) {
 Mutex::Autolock _l(mCblk->lock);
 if (mCblk->clientIndex > EFFECT_PARAM_BUFFER_SIZE ||
            mCblk->serverIndex > EFFECT_PARAM_BUFFER_SIZE) {
            mCblk->serverIndex = 0;
            mCblk->clientIndex = 0;
 return BAD_VALUE;
 }
 status_t status = NO_ERROR;
 while (mCblk->serverIndex < mCblk->clientIndex) {
 int reply;
 uint32_t rsize = sizeof(int);
 int *p = (int *)(mBuffer + mCblk->serverIndex);
 int size = *p++;
 if (((uint8_t *)p + size) > mBuffer + mCblk->clientIndex) {
                ALOGW("command(): invalid parameter block size");
 break;
 }
 effect_param_t *param = (effect_param_t *)p;
 if (param->psize == 0 || param->vsize == 0) {
                ALOGW("command(): null parameter or value size");
                mCblk->serverIndex += size;
 continue;
 }
 uint32_t psize = sizeof(effect_param_t) +
 ((param->psize - 1) / sizeof(int) + 1) * sizeof(int) +
                             param->vsize;
 status_t ret = mEffect->command(EFFECT_CMD_SET_PARAM,
                                            psize,
                                            p,
 &rsize,
 &reply);
 if (ret != NO_ERROR) {
                status = ret;
 *(int *)pReplyData = reply;
 break;
 } else if (reply != NO_ERROR) {
 *(int *)pReplyData = reply;
 break;
 }
            mCblk->serverIndex += size;
 }
        mCblk->serverIndex = 0;
        mCblk->clientIndex = 0;
 return status;
 } else if (cmdCode == EFFECT_CMD_ENABLE) {
 *(int *)pReplyData = NO_ERROR;
 return enable();
 } else if (cmdCode == EFFECT_CMD_DISABLE) {
 *(int *)pReplyData = NO_ERROR;
 return disable();
 }

 return mEffect->command(cmdCode, cmdSize, pCmdData, replySize, pReplyData);
}
