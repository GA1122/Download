VideoCaptureManager::CaptureDeviceStartRequest::CaptureDeviceStartRequest(
    VideoCaptureController* controller,
    media::VideoCaptureSessionId session_id,
    const media::VideoCaptureParams& params)
    : controller_(controller), session_id_(session_id), params_(params) {}
