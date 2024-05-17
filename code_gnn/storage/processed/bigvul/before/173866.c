 virtual status_t signalEndOfInputStream(node_id node) {
 Parcel data, reply;
 status_t err;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        data.writeInt32((int32_t)node);
        err = remote()->transact(SIGNAL_END_OF_INPUT_STREAM, data, &reply);
 if (err != OK) {
            ALOGW("binder transaction failed: %d", err);
 return err;
 }

 return reply.readInt32();
 }
