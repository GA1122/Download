PassRefPtr<SharedBuffer> readFile(const char* fileName)
{
    FILE* fp = fopen(fileName, "rb");
    if (!fp) {
        fprintf(stderr, "Can't open file %s\n", fileName);
        exit(2);
    }

    sttype s;
    stat(fileName, &s);
    size_t fileSize = s.st_size;
    if (s.st_size <= 0)
        return SharedBuffer::create();

    OwnPtr<unsigned char[]> buffer = adoptArrayPtr(new unsigned char[fileSize]);
    if (fileSize != fread(buffer.get(), 1, fileSize, fp)) {
        fprintf(stderr, "Error reading file %s\n", fileName);
        exit(2);
    }

    fclose(fp);
    return SharedBuffer::create(buffer.get(), fileSize);
}
