static void dump_backtrace_for_thread(HANDLE hThread)
{
    STACKFRAME sf;
    CONTEXT context;
    DWORD dwImageType;

    if (!pStackWalk)
        if (!init_backtrace())
            return;

     
    if (hThread == GetCurrentThread())
        return;

    DPRINTF("Backtrace:\n");

    _DBUS_ZERO(context);
    context.ContextFlags = CONTEXT_FULL;

    SuspendThread(hThread);

    if (!GetThreadContext(hThread, &context))
    {
        DPRINTF("Couldn't get thread context (error %ld)\n", GetLastError());
        ResumeThread(hThread);
        return;
    }

    _DBUS_ZERO(sf);

#ifdef __i386__
    sf.AddrFrame.Offset = context.Ebp;
    sf.AddrFrame.Mode = AddrModeFlat;
    sf.AddrPC.Offset = context.Eip;
    sf.AddrPC.Mode = AddrModeFlat;
    dwImageType = IMAGE_FILE_MACHINE_I386;
#elif _M_X64
  dwImageType                = IMAGE_FILE_MACHINE_AMD64;
  sf.AddrPC.Offset    = context.Rip;
  sf.AddrPC.Mode      = AddrModeFlat;
  sf.AddrFrame.Offset = context.Rsp;
  sf.AddrFrame.Mode   = AddrModeFlat;
  sf.AddrStack.Offset = context.Rsp;
  sf.AddrStack.Mode   = AddrModeFlat;
#elif _M_IA64
  dwImageType                 = IMAGE_FILE_MACHINE_IA64;
  sf.AddrPC.Offset    = context.StIIP;
  sf.AddrPC.Mode      = AddrModeFlat;
  sf.AddrFrame.Offset = context.IntSp;
  sf.AddrFrame.Mode   = AddrModeFlat;
  sf.AddrBStore.Offset= context.RsBSP;
  sf.AddrBStore.Mode  = AddrModeFlat;
  sf.AddrStack.Offset = context.IntSp;
  sf.AddrStack.Mode   = AddrModeFlat;
#else
# error You need to fill in the STACKFRAME structure for your architecture
#endif

    while (pStackWalk(dwImageType, GetCurrentProcess(),
                     hThread, &sf, &context, NULL, pSymFunctionTableAccess,
                     pSymGetModuleBase, NULL))
    {
        BYTE buffer[256];
        IMAGEHLP_SYMBOL * pSymbol = (IMAGEHLP_SYMBOL *)buffer;
        DWORD dwDisplacement;

        pSymbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
        pSymbol->MaxNameLength = sizeof(buffer) - sizeof(IMAGEHLP_SYMBOL) + 1;

        if (!pSymGetSymFromAddr(GetCurrentProcess(), sf.AddrPC.Offset,
                                &dwDisplacement, pSymbol))
        {
            IMAGEHLP_MODULE ModuleInfo;
            ModuleInfo.SizeOfStruct = sizeof(ModuleInfo);

            if (!pSymGetModuleInfo(GetCurrentProcess(), sf.AddrPC.Offset,
                                   &ModuleInfo))
                DPRINTF("1\t%p\n", (void*)sf.AddrPC.Offset);
            else
                DPRINTF("2\t%s+0x%lx\n", ModuleInfo.ImageName,
                    sf.AddrPC.Offset - ModuleInfo.BaseOfImage);
        }
        else if (dwDisplacement)
            DPRINTF("3\t%s+0x%lx\n", pSymbol->Name, dwDisplacement);
        else
            DPRINTF("4\t%s\n", pSymbol->Name);
    }

    ResumeThread(hThread);
}
