 virtual status_t createPersistentInputSurface(
            sp<IGraphicBufferProducer> *bufferProducer,
            sp<IGraphicBufferConsumer> *bufferConsumer) {
 Parcel data, reply;
 status_t err;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        err = remote()->transact(CREATE_PERSISTENT_INPUT_SURFACE, data, &reply);
 if (err != OK) {
            ALOGW("binder transaction failed: %d", err);
 return err;
 }

        err = reply.readInt32();
 if (err != OK) {
 return err;
 }

 *bufferProducer = IGraphicBufferProducer::asInterface(
                reply.readStrongBinder());
 *bufferConsumer = IGraphicBufferConsumer::asInterface(
                reply.readStrongBinder());

 return err;
 }
