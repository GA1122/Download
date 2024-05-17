 virtual int32_t setHeap(const sp<IMemoryHeap> &heap) {
 Parcel data, reply;
        data.writeInterfaceToken(ICrypto::getInterfaceDescriptor());
        data.writeStrongBinder(IInterface::asBinder(heap));
 status_t err = remote()->transact(SET_HEAP, data, &reply);
 if (err != NO_ERROR) {
 return -1;
 }
 int32_t seqNum;
 if (reply.readInt32(&seqNum) != NO_ERROR) {
 return -1;
 }
 return seqNum;
 }
