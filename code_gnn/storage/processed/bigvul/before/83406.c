HandleRegisterDNSMessage(void)
{
    DWORD err;
    HANDLE thread = NULL;

     
    thread = CreateThread(NULL, 0, RegisterDNS, NULL, 0, NULL);

     
    if (thread)
    {
        err = 0;
        CloseHandle(thread);
    }
    else
    {
        err = GetLastError();
    }

    return err;
}
