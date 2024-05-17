    void writeFileList(const FileList& fileList)
    {
        append(FileListTag);
        uint32_t length = fileList.length();
        doWriteUint32(length);
        for (unsigned i = 0; i < length; ++i)
            doWriteFile(*fileList.item(i));
    }
