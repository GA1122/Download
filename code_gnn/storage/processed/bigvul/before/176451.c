FrameSequence_gif::FrameSequence_gif(Stream* stream) :
        mLoopCount(1), mBgColor(TRANSPARENT), mPreservedFrames(NULL), mRestoringFrames(NULL) {
    mGif = DGifOpen(stream, streamReader, NULL);
 if (!mGif) {
        ALOGW("Gif load failed");
 return;
 }

 if (DGifSlurp(mGif) != GIF_OK) {
        ALOGW("Gif slurp failed");
 DGifCloseFile(mGif, NULL);
        mGif = NULL;
 return;
 }

 long durationMs = 0;
 int lastUnclearedFrame = -1;
    mPreservedFrames = new bool[mGif->ImageCount];
    mRestoringFrames = new int[mGif->ImageCount];

 GraphicsControlBlock gcb;
 for (int i = 0; i < mGif->ImageCount; i++) {
 const SavedImage& image = mGif->SavedImages[i];

 for (int j = 0; (j + 1) < image.ExtensionBlockCount; j++) {
 ExtensionBlock* eb1 = image.ExtensionBlocks + j;
 ExtensionBlock* eb2 = image.ExtensionBlocks + j + 1;
 if (eb1->Function == APPLICATION_EXT_FUNC_CODE
 && eb1->ByteCount == 11
 && !memcmp((const char*)(eb1->Bytes), "NETSCAPE2.0", 11)
 && eb2->Function == CONTINUE_EXT_FUNC_CODE
 && eb2->ByteCount == 3
 && eb2->Bytes[0] == 1) {
                mLoopCount = (int)(eb2->Bytes[2] << 8) + (int)(eb2->Bytes[1]);
 }
 }

 DGifSavedExtensionToGCB(mGif, i, &gcb);

        durationMs += getDelayMs(gcb);

        mPreservedFrames[i] = false;
        mRestoringFrames[i] = -1;
 if (gcb.DisposalMode == DISPOSE_PREVIOUS && lastUnclearedFrame >= 0) {
            mPreservedFrames[lastUnclearedFrame] = true;
            mRestoringFrames[i] = lastUnclearedFrame;
 }
 if (!willBeCleared(gcb)) {
            lastUnclearedFrame = i;
 }
 }

#if GIF_DEBUG
    ALOGD("FrameSequence_gif created with size %d %d, frames %d dur %ld",
            mGif->SWidth, mGif->SHeight, mGif->ImageCount, durationMs);
 for (int i = 0; i < mGif->ImageCount; i++) {
 DGifSavedExtensionToGCB(mGif, i, &gcb);
        ALOGD("    Frame %d - must preserve %d, restore point %d, trans color %d",
                i, mPreservedFrames[i], mRestoringFrames[i], gcb.TransparentColor);
 }
#endif

 if (mGif->SColorMap) {
 GraphicsControlBlock gcb;
 DGifSavedExtensionToGCB(mGif, 0, &gcb);
 if (gcb.TransparentColor == NO_TRANSPARENT_COLOR) {
            mBgColor = gifColorToColor8888(mGif->SColorMap->Colors[mGif->SBackGroundColor]);
 }
 }
}
