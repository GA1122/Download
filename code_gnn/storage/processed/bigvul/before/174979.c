CameraClient::~CameraClient() {
 Mutex* lock = mCameraService->getClientLockById(mCameraId);
    lock->lock();
    mDestructionStarted = true;
    lock->unlock();
 int callingPid = getCallingPid();
    LOG1("CameraClient::~CameraClient E (pid %d, this %p)", callingPid, this);

    disconnect();
    LOG1("CameraClient::~CameraClient X (pid %d, this %p)", callingPid, this);

 }
