status_t ProCamera2Client::getCameraInfo(int cameraId,
  
                                         camera_metadata** info)
{
 if (cameraId != mCameraId) {
 return INVALID_OPERATION;
 }

 Mutex::Autolock icl(mBinderSerializationLock);

 if (!mDevice.get()) return DEAD_OBJECT;

 CameraMetadata deviceInfo = mDevice->info();
 *info = deviceInfo.release();

 return OK;

 }
