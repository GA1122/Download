 virtual status_t setCaptureState(bool active)
 {
 Parcel data, reply;
        data.writeInterfaceToken(ISoundTriggerHwService::getInterfaceDescriptor());
        data.writeInt32(active);
 status_t status = remote()->transact(SET_CAPTURE_STATE, data, &reply);
 if (status == NO_ERROR) {
            status = reply.readInt32();
 }
 return status;
 }
