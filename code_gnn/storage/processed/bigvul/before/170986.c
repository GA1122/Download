void AudioFlinger::EffectHandle::commandExecuted(uint32_t cmdCode,
 uint32_t cmdSize,
 void *pCmdData,
 uint32_t replySize,
 void *pReplyData)
{
 if (mEffectClient != 0) {
        mEffectClient->commandExecuted(cmdCode, cmdSize, pCmdData, replySize, pReplyData);
 }
}
