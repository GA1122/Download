 virtual void onMessages(const std::list<omx_message> &messages) {
 Parcel data, reply;
        std::list<omx_message>::const_iterator it = messages.cbegin();
 bool first = true;
 while (it != messages.cend()) {
 const omx_message &msg = *it++;
 if (first) {
                data.writeInterfaceToken(IOMXObserver::getInterfaceDescriptor());
                data.writeInt32(msg.node);
                first = false;
 }
            data.writeInt32(msg.fenceFd >= 0);
 if (msg.fenceFd >= 0) {
                data.writeFileDescriptor(msg.fenceFd, true  );
 }
            data.writeInt32(msg.type);
            data.write(&msg.u, sizeof(msg.u));
            ALOGV("onMessage writing message %d, size %zu", msg.type, sizeof(msg));
 }
 if (!first) {
            data.writeInt32(-1);  
            remote()->transact(OBSERVER_ON_MSG, data, &reply, IBinder::FLAG_ONEWAY);
 }
 }
