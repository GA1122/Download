    PassRefPtrWillBeRawPtr<File> readFileHelper()
    {
        if (m_version < 3)
            return nullptr;
        ASSERT(!m_blobInfo);
        String path;
        String name;
        String relativePath;
        String uuid;
        String type;
        uint32_t hasSnapshot = 0;
        uint64_t size = 0;
        double lastModified = 0;
        if (!readWebCoreString(&path))
            return nullptr;
        if (m_version >= 4 && !readWebCoreString(&name))
            return nullptr;
        if (m_version >= 4 && !readWebCoreString(&relativePath))
            return nullptr;
        if (!readWebCoreString(&uuid))
            return nullptr;
        if (!readWebCoreString(&type))
            return nullptr;
        if (m_version >= 4 && !doReadUint32(&hasSnapshot))
            return nullptr;
        if (hasSnapshot) {
            if (!doReadUint64(&size))
                return nullptr;
            if (!doReadNumber(&lastModified))
                return nullptr;
        }
        return File::createFromSerialization(path, name, relativePath, hasSnapshot > 0, size, lastModified, getOrCreateBlobDataHandle(uuid, type));
    }
