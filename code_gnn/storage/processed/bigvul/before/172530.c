CameraSource::~CameraSource() {
 if (mStarted) {
        reset();
 } else if (mInitCheck == OK) {
        releaseCamera();
 }
}
