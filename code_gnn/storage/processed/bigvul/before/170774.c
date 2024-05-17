 virtual void notify(
 int msg, int ext1, int ext2, const Parcel *obj) {
 Parcel data, reply;
        data.writeInterfaceToken(IHDCPObserver::getInterfaceDescriptor());
        data.writeInt32(msg);
        data.writeInt32(ext1);
        data.writeInt32(ext2);
 if (obj && obj->dataSize() > 0) {
            data.appendFrom(const_cast<Parcel *>(obj), 0, obj->dataSize());
 }
        remote()->transact(OBSERVER_NOTIFY, data, &reply, IBinder::FLAG_ONEWAY);
 }
