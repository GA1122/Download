PolicyKitBackend::PolicyKitBackend()
    : AuthBackend()
{
    setCapabilities(AuthorizeFromClientCapability);
}
