ReturnProcessId(HANDLE pipe, DWORD pid, DWORD count, LPHANDLE events)
{
    const WCHAR msg[] = L"Process ID";
    WCHAR buf[22 + _countof(msg)];  

     
    swprintf(buf, _countof(buf), L"0x%08x\n0x%08x\n%s", 0, pid, msg);
    buf[_countof(buf) - 1] = '\0';

    WritePipeAsync(pipe, buf, (DWORD)(wcslen(buf) * 2), count, events);
}
