bool xmp_serialize_and_format(XmpPtr xmp, XmpStringPtr buffer, uint32_t options,
                              uint32_t padding, const char *newline,
                              const char *tab, int32_t indent)
{
    CHECK_PTR(xmp, false);
    CHECK_PTR(buffer, false);
    RESET_ERROR;

    SXMPMeta *txmp = (SXMPMeta *)xmp;
    try {
        txmp->SerializeToBuffer(STRING(buffer), options, padding, newline, tab,
                                indent);
    }
    catch (const XMP_Error &e) {
        set_error(e);
        return false;
    }
    return true;
}
