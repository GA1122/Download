status_t FLACSource::read(
 MediaBuffer **outBuffer, const ReadOptions *options)
{
 MediaBuffer *buffer;
 int64_t seekTimeUs;
 ReadOptions::SeekMode mode;
 if ((NULL != options) && options->getSeekTo(&seekTimeUs, &mode)) {
        FLAC__uint64 sample;
 if (seekTimeUs <= 0LL) {
            sample = 0LL;
 } else {
            sample = (seekTimeUs * mParser->getSampleRate()) / 1000000LL;
 if (sample >= mParser->getTotalSamples()) {
                sample = mParser->getTotalSamples();
 }
 }
        buffer = mParser->readBuffer(sample);
 } else {
        buffer = mParser->readBuffer();
 }
 *outBuffer = buffer;
 return buffer != NULL ? (status_t) OK : (status_t) ERROR_END_OF_STREAM;
}
