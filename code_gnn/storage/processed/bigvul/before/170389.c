MPEG4Source::~MPEG4Source() {
 if (mStarted) {
        stop();
 }
    free(mCurrentSampleInfoSizes);
    free(mCurrentSampleInfoOffsets);
}
