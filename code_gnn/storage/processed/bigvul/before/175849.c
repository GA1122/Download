 virtual status_t attach(const sound_trigger_module_handle_t handle,
 const sp<ISoundTriggerClient>& client,
                            sp<ISoundTrigger>& module)
 {
 Parcel data, reply;
        data.writeInterfaceToken(ISoundTriggerHwService::getInterfaceDescriptor());
        data.write(&handle, sizeof(sound_trigger_module_handle_t));
        data.writeStrongBinder(IInterface::asBinder(client));
        remote()->transact(ATTACH, data, &reply);
 status_t status = reply.readInt32();
 if (reply.readInt32() != 0) {
            module = interface_cast<ISoundTrigger>(reply.readStrongBinder());
 }
 return status;
 }
