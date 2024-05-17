void V8Proxy::clearForNavigation()
{
    resetIsolatedWorlds();
    windowShell()->clearForNavigation();
}
