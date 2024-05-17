static double currentSystemTime()
{
    FILETIME ft;
    GetCurrentFT(&ft);


    ULARGE_INTEGER t;
    memcpy(&t, &ft, sizeof(t));



    return t.QuadPart * 0.0000001 - 11644473600.0;
}
