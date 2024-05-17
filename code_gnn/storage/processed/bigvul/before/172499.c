 void stopRecording()
 {
        ALOGV("stopRecording");
 Parcel data, reply;
        data.writeInterfaceToken(ICameraRecordingProxy::getInterfaceDescriptor());
        remote()->transact(STOP_RECORDING, data, &reply);
 }
