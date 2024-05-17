static V8Extensions& staticExtensionsList()
{
    DEFINE_STATIC_LOCAL(V8Extensions, extensions, ());
    return extensions;
}
