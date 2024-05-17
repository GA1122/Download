    void writeObjectReference(uint32_t reference)
    {
        append(ObjectReferenceTag);
        doWriteUint32(reference);
    }
