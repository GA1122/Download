void Visualizer_reset(VisualizerContext *pContext)
{
    pContext->mCaptureIdx = 0;
    pContext->mLastCaptureIdx = 0;
    pContext->mBufferUpdateTime.tv_sec = 0;
    pContext->mLatency = 0;
    memset(pContext->mCaptureBuf, 0x80, CAPTURE_BUF_SIZE);
}
