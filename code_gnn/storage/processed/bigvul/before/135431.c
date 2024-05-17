void Document::updateSecurityOrigin(PassRefPtr<SecurityOrigin> origin)
{
    setSecurityOrigin(origin);
    didUpdateSecurityOrigin();
}
