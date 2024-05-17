 void releaseRecordingFrame(const sp<IMemory>& mem)
 {
        ALOGV("releaseRecordingFrame");
 Parcel data, reply;
        data.writeInterfaceToken(ICameraRecordingProxy::getInterfaceDescriptor());
        data.writeStrongBinder(IInterface::asBinder(mem));
        remote()->transact(RELEASE_RECORDING_FRAME, data, &reply);
 }
