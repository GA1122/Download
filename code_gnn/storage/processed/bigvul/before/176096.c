 virtual ssize_t decrypt(const uint8_t key[16], const uint8_t iv[16],
 CryptoPlugin::Mode mode, const CryptoPlugin::Pattern &pattern,
 const SourceBuffer &source, size_t offset,
 const CryptoPlugin::SubSample *subSamples, size_t numSubSamples,
 const DestinationBuffer &destination, AString *errorDetailMsg) {
 Parcel data, reply;
        data.writeInterfaceToken(ICrypto::getInterfaceDescriptor());
        data.writeInt32(mode);
        data.writeInt32(pattern.mEncryptBlocks);
        data.writeInt32(pattern.mSkipBlocks);

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
        data.writeStrongBinder(IInterface::asBinder(source.mSharedMemory));
        data.writeInt32(source.mHeapSeqNum);
        data.writeInt32(offset);

        data.writeInt32(numSubSamples);
        data.write(subSamples, sizeof(CryptoPlugin::SubSample) * numSubSamples);

        data.writeInt32((int32_t)destination.mType);
 if (destination.mType == kDestinationTypeNativeHandle) {
 if (destination.mHandle == NULL) {
 return BAD_VALUE;
 }
            data.writeNativeHandle(destination.mHandle);
 } else {
 if (destination.mSharedMemory == NULL) {
 return BAD_VALUE;
 }
            data.writeStrongBinder(IInterface::asBinder(destination.mSharedMemory));
 }

        remote()->transact(DECRYPT, data, &reply);

 ssize_t result = reply.readInt32();

 if (isCryptoError(result)) {
 AString msg = reply.readCString();
 if (errorDetailMsg) {
 *errorDetailMsg = msg;
 }
 }

 return result;
 }
