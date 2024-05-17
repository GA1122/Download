 virtual ssize_t decrypt(
 bool secure,
 const uint8_t key[16],
 const uint8_t iv[16],
 CryptoPlugin::Mode mode,
 const sp<IMemory> &sharedBuffer, size_t offset,
 const CryptoPlugin::SubSample *subSamples, size_t numSubSamples,
 void *dstPtr,
 AString *errorDetailMsg) {
 Parcel data, reply;
        data.writeInterfaceToken(ICrypto::getInterfaceDescriptor());
        data.writeInt32(secure);
        data.writeInt32(mode);

 static const uint8_t kDummy[16] = { 0 };

 if (key == NULL) {
            key = kDummy;
 }

 if (iv == NULL) {
            iv = kDummy;
 }

        data.write(key, 16);
        data.write(iv, 16);

 size_t totalSize = 0;
 for (size_t i = 0; i < numSubSamples; ++i) {
            totalSize += subSamples[i].mNumBytesOfEncryptedData;
            totalSize += subSamples[i].mNumBytesOfClearData;
 }

        data.writeInt32(totalSize);
        data.writeStrongBinder(IInterface::asBinder(sharedBuffer));
        data.writeInt32(offset);

        data.writeInt32(numSubSamples);
        data.write(subSamples, sizeof(CryptoPlugin::SubSample) * numSubSamples);

 if (secure) {
            data.writeInt64(static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dstPtr)));
 }

        remote()->transact(DECRYPT, data, &reply);

 ssize_t result = reply.readInt32();

 if (isCryptoError(result)) {
            errorDetailMsg->setTo(reply.readCString());
 }

 if (!secure && result >= 0) {
            reply.read(dstPtr, result);
 }

 return result;
 }
