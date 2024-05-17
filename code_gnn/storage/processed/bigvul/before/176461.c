void FrameSequenceState_gif::restorePreserveBuffer(Color8888* outputPtr, int outputPixelStride) {
 const int width = mFrameSequence.getWidth();
 const int height = mFrameSequence.getHeight();
 if (!mPreserveBuffer) {
        ALOGD("preserve buffer not allocated! ah!");
 return;
 }
 for (int y = 0; y < height; y++) {
        memcpy(outputPtr + outputPixelStride * y,
                mPreserveBuffer + width * y,
                width * 4);
 }
}
