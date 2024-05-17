void CameraService::Client::disconnect() {
    ALOGV("Client::disconnect");
 BasicClient::disconnect();
    mCameraService->setCameraFree(mCameraId);

 StatusVector rejectSourceStates;
    rejectSourceStates.push_back(ICameraServiceListener::STATUS_NOT_PRESENT);
    rejectSourceStates.push_back(ICameraServiceListener::STATUS_ENUMERATING);

    mCameraService->updateStatus(ICameraServiceListener::STATUS_PRESENT,
                                 mCameraId,
 &rejectSourceStates);
}
