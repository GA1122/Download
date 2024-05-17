status_t BnHDCPObserver::onTransact(
 uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags) {
 switch (code) {
 case OBSERVER_NOTIFY:
 {
            CHECK_INTERFACE(IHDCPObserver, data, reply);

 int msg = data.readInt32();
 int ext1 = data.readInt32();
 int ext2 = data.readInt32();

 Parcel obj;
 if (data.dataAvail() > 0) {
                obj.appendFrom(
 const_cast<Parcel *>(&data),
                        data.dataPosition(),
                        data.dataAvail());
 }

            notify(msg, ext1, ext2, &obj);

 return OK;
 }

 default:
 return BBinder::onTransact(code, data, reply, flags);
 }
}
