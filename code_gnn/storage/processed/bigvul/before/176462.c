void FrameSequenceState_gif::savePreserveBuffer(Color8888* outputPtr, int outputPixelStride, int frameNr) {
 if (frameNr == mPreserveBufferFrame) return;

    mPreserveBufferFrame = frameNr;
 const int width = mFrameSequence.getWidth();
 const int height = mFrameSequence.getHeight();
 if (!mPreserveBuffer) {
        mPreserveBuffer = new Color8888[width * height];
 }
 for (int y = 0; y < height; y++) {
        memcpy(mPreserveBuffer + width * y,
                outputPtr + outputPixelStride * y,
                width * 4);
 }
}
