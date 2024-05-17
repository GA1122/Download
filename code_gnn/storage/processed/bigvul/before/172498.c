 status_t startRecording(const sp<ICameraRecordingProxyListener>& listener)
 {
        ALOGV("startRecording");
 Parcel data, reply;
        data.writeInterfaceToken(ICameraRecordingProxy::getInterfaceDescriptor());
        data.writeStrongBinder(IInterface::asBinder(listener));
        remote()->transact(START_RECORDING, data, &reply);
 return reply.readInt32();
 }
