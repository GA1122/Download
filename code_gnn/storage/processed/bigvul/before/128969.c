    bool readBlob(v8::Handle<v8::Value>* value, bool isIndexed)
    {
        if (m_version < 3)
            return false;
        RefPtrWillBeRawPtr<Blob> blob;
        if (isIndexed) {
            if (m_version < 6)
                return false;
            ASSERT(m_blobInfo);
            uint32_t index;
            if (!doReadUint32(&index) || index >= m_blobInfo->size())
                return false;
            const blink::WebBlobInfo& info = (*m_blobInfo)[index];
            blob = Blob::create(getOrCreateBlobDataHandle(info.uuid(), info.type(), info.size()));
        } else {
            ASSERT(!m_blobInfo);
            String uuid;
            String type;
            uint64_t size;
            ASSERT(!m_blobInfo);
            if (!readWebCoreString(&uuid))
                return false;
            if (!readWebCoreString(&type))
                return false;
            if (!doReadUint64(&size))
                return false;
            blob = Blob::create(getOrCreateBlobDataHandle(uuid, type, size));
        }
        *value = toV8(blob.release(), m_scriptState->context()->Global(), isolate());
        return true;
    }
