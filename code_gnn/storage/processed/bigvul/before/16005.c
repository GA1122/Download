XmpPtr xmp_copy(XmpPtr xmp)
{
    CHECK_PTR(xmp, NULL);
    RESET_ERROR;

    try {
        auto txmp = std::unique_ptr<SXMPMeta>(new SXMPMeta(*(SXMPMeta *)xmp));
        return reinterpret_cast<XmpPtr>(txmp.release());
    }
    catch (const XMP_Error &e) {
        set_error(e);
    }
    return NULL;
}
