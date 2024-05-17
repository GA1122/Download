 virtual void onMessage(const omx_message &msg) {
 Parcel data, reply;
        data.writeInterfaceToken(IOMXObserver::getInterfaceDescriptor());
        data.write(&msg, sizeof(msg));

        ALOGV("onMessage writing message %d, size %zu", msg.type, sizeof(msg));

        remote()->transact(OBSERVER_ON_MSG, data, &reply, IBinder::FLAG_ONEWAY);
 }
