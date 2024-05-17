status_t Camera2Client::commandPlayRecordingSoundL() {
    mCameraService->playSound(CameraService::SOUND_RECORDING);
 return OK;
}
