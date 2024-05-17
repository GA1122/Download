bool xmp_parse(XmpPtr xmp, const char *buffer, size_t len)
{
    CHECK_PTR(xmp, false);
    CHECK_PTR(buffer, false);

    SXMPMeta *txmp = (SXMPMeta *)xmp;
    try {
        txmp->ParseFromBuffer(buffer, len, kXMP_RequireXMPMeta);
    }
    catch (const XMP_Error &e) {
        set_error(e);
        return false;
    }
    return true;
}
