AMRSource::~AMRSource() {
 if (mStarted) {
        stop();
 }
}
