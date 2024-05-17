 virtual status_t setGenerationNumber(uint32_t generationNumber) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferProducer::getInterfaceDescriptor());
        data.writeUint32(generationNumber);
 status_t result = remote()->transact(SET_GENERATION_NUMBER, data, &reply);
 if (result == NO_ERROR) {
            result = reply.readInt32();
 }
 return result;
 }
