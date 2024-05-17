void Camera3Device::notify(const camera3_notify_msg *msg) {
    ATRACE_CALL();
 NotificationListener *listener;
 {
 Mutex::Autolock l(mOutputLock);
        listener = mListener;
 }

 if (msg == NULL) {
        SET_ERR("HAL sent NULL notify message!");
 return;
 }

 switch (msg->type) {
 case CAMERA3_MSG_ERROR: {
            notifyError(msg->message.error, listener);
 break;
 }
 case CAMERA3_MSG_SHUTTER: {
            notifyShutter(msg->message.shutter, listener);
 break;
 }
 default:
            SET_ERR("Unknown notify message from HAL: %d",
                    msg->type);
 }
}
