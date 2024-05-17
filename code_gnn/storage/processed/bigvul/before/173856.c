 virtual status_t listNodes(List<ComponentInfo> *list) {
 list->clear();

 Parcel data, reply;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        remote()->transact(LIST_NODES, data, &reply);

 int32_t n = reply.readInt32();
 for (int32_t i = 0; i < n; ++i) {
 list->push_back(ComponentInfo());
 ComponentInfo &info = *--list->end();

            info.mName = reply.readString8();
 int32_t numRoles = reply.readInt32();
 for (int32_t j = 0; j < numRoles; ++j) {
                info.mRoles.push_back(reply.readString8());
 }
 }

 return OK;
 }
