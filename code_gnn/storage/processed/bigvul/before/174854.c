Mutex* CameraService::Client::getClientLockFromCookie(void* user) {
 return gCameraService->getClientLockById((int) user);
}
