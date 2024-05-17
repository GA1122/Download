    void writeFile(const File& file)
    {
        append(FileTag);
        doWriteFile(file);
    }
