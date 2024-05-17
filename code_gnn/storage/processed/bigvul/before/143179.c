String Document::domain() const
{
    return getSecurityOrigin()->domain();
}
