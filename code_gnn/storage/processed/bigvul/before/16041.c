XmpPtr xmp_new(const char *buffer, size_t len)
{
    CHECK_PTR(buffer, NULL);
    RESET_ERROR;

    try {
        auto txmp = std::unique_ptr<SXMPMeta>(new SXMPMeta(buffer, len));
        return reinterpret_cast<XmpPtr>(txmp.release());
    }
    catch (const XMP_Error &e) {
        set_error(e);
    }
    return NULL;
}
