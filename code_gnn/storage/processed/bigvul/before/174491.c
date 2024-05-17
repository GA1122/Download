status_t BnOMXObserver::onTransact(
 uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags) {
 switch (code) {
 case OBSERVER_ON_MSG:
 {
            CHECK_OMX_INTERFACE(IOMXObserver, data, reply);

            omx_message msg;
            data.read(&msg, sizeof(msg));

            ALOGV("onTransact reading message %d, size %zu", msg.type, sizeof(msg));

            onMessage(msg);

 return NO_ERROR;
 }

 default:
 return BBinder::onTransact(code, data, reply, flags);
 }
}
