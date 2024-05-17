 virtual status_t allocateNode(
 const char *name, const sp<IOMXObserver> &observer, node_id *node) {
 Parcel data, reply;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        data.writeCString(name);
        data.writeStrongBinder(observer->asBinder());
        remote()->transact(ALLOCATE_NODE, data, &reply);

 status_t err = reply.readInt32();
 if (err == OK) {
 *node = (node_id)reply.readInt32();
 } else {
 *node = 0;
 }

 return err;
 }
