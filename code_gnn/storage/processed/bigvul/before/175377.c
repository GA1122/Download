OggSource::~OggSource() {
 if (mStarted) {
        stop();
 }
}
