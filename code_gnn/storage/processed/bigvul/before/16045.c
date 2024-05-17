bool xmp_register_namespace(const char *namespaceURI,
                            const char *suggestedPrefix,
                            XmpStringPtr registeredPrefix)
{
    RESET_ERROR;
    try {
        return SXMPMeta::RegisterNamespace(namespaceURI, suggestedPrefix,
                                           STRING(registeredPrefix));
    }
    catch (const XMP_Error &e) {
        set_error(e);
    }
    return false;
}
