bool xmp_prefix_namespace_uri(const char *prefix, XmpStringPtr ns)
{
    CHECK_PTR(prefix, false);
    RESET_ERROR;
    try {
        return SXMPMeta::GetNamespaceURI(prefix, STRING(ns));
    }
    catch (const XMP_Error &e) {
        set_error(e);
    }
    return false;
}
