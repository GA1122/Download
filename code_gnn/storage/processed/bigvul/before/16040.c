bool xmp_namespace_prefix(const char *ns, XmpStringPtr prefix)
{
    CHECK_PTR(ns, false);
    RESET_ERROR;
    try {
        return SXMPMeta::GetNamespacePrefix(ns, STRING(prefix));
    }
    catch (const XMP_Error &e) {
        set_error(e);
    }
    return false;
}
