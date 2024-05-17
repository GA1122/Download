void ATSParser::Stream::signalEOS(status_t finalResult) {
 if (mSource != NULL) {
        mSource->signalEOS(finalResult);
 }
    mEOSReached = true;
    flush(NULL);
}
