status_t CameraService::onTransact(
 uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags) {
 switch (code) {
 case BnCameraService::CONNECT:
 case BnCameraService::CONNECT_PRO:
 case BnCameraService::CONNECT_DEVICE:
 const int pid = getCallingPid();
 const int self_pid = getpid();
 if (pid != self_pid) {
 if (!checkCallingPermission(
 String16("android.permission.CAMERA"))) {
 const int uid = getCallingUid();
                    ALOGE("Permission Denial: "
 "can't use the camera pid=%d, uid=%d", pid, uid);
 return PERMISSION_DENIED;
 }
 }
 break;
 }

 return BnCameraService::onTransact(code, data, reply, flags);
}
