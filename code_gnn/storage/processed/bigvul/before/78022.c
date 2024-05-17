cmsBool BuildAbsolutePath(const char *relPath, const char *basePath, char *buffer, cmsUInt32Number MaxLen)
{
    char *tail;
    cmsUInt32Number len;

    if (isabsolutepath(relPath)) {

        strncpy(buffer, relPath, MaxLen);
        buffer[MaxLen-1] = 0;
        return TRUE;
    }

    strncpy(buffer, basePath, MaxLen);
    buffer[MaxLen-1] = 0;

    tail = strrchr(buffer, DIR_CHAR);
    if (tail == NULL) return FALSE;     

    len = (cmsUInt32Number) (tail - buffer);
    if (len >= MaxLen) return FALSE;

    strncpy(tail + 1, relPath, MaxLen - len);

    return TRUE;
}
