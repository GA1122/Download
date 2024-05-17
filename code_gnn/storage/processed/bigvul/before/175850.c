 virtual status_t listModules(struct sound_trigger_module_descriptor *modules,
 uint32_t *numModules)
 {
 if (numModules == NULL || (*numModules != 0 && modules == NULL)) {
 return BAD_VALUE;
 }
 Parcel data, reply;
        data.writeInterfaceToken(ISoundTriggerHwService::getInterfaceDescriptor());
 unsigned int numModulesReq = (modules == NULL) ? 0 : *numModules;
        data.writeInt32(numModulesReq);
 status_t status = remote()->transact(LIST_MODULES, data, &reply);
 if (status == NO_ERROR) {
            status = (status_t)reply.readInt32();
 *numModules = (unsigned int)reply.readInt32();
 }
        ALOGV("listModules() status %d got *numModules %d", status, *numModules);
 if (status == NO_ERROR) {
 if (numModulesReq > *numModules) {
                numModulesReq = *numModules;
 }
 if (numModulesReq > 0) {
                reply.read(modules, numModulesReq * sizeof(struct sound_trigger_module_descriptor));
 }
 }
 return status;
 }
