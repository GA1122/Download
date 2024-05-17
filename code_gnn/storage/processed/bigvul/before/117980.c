void V8Proxy::clearForClose()
{
    resetIsolatedWorlds();
    windowShell()->clearForClose();
}
