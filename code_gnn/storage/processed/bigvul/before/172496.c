status_t BnCameraRecordingProxy::onTransact(
 uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
 switch(code) {
 case START_RECORDING: {
            ALOGV("START_RECORDING");
            CHECK_INTERFACE(ICameraRecordingProxy, data, reply);
            sp<ICameraRecordingProxyListener> listener =
                interface_cast<ICameraRecordingProxyListener>(data.readStrongBinder());
            reply->writeInt32(startRecording(listener));
 return NO_ERROR;
 } break;
 case STOP_RECORDING: {
            ALOGV("STOP_RECORDING");
            CHECK_INTERFACE(ICameraRecordingProxy, data, reply);
            stopRecording();
 return NO_ERROR;
 } break;
 case RELEASE_RECORDING_FRAME: {
            ALOGV("RELEASE_RECORDING_FRAME");
            CHECK_INTERFACE(ICameraRecordingProxy, data, reply);
            sp<IMemory> mem = interface_cast<IMemory>(data.readStrongBinder());
            releaseRecordingFrame(mem);
 return NO_ERROR;
 } break;

 default:
 return BBinder::onTransact(code, data, reply, flags);
 }
}
