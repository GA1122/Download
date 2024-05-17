AudioSource::~AudioSource() {
 if (mStarted) {
        reset();
 }
}
