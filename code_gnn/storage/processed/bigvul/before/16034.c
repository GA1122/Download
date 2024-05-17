bool xmp_get_property_int64(XmpPtr xmp, const char *schema, const char *name,
                            int64_t *property, uint32_t *propsBits)
{
    CHECK_PTR(xmp, false);
    RESET_ERROR;

    bool ret = false;
    try {
        auto txmp = reinterpret_cast<const SXMPMeta *>(xmp);
        XMP_OptionBits optionBits;
        ret = txmp->GetProperty_Int64(schema, name, property, &optionBits);
        if (propsBits) {
            *propsBits = optionBits;
        }
    }
    catch (const XMP_Error &e) {
        set_error(e);
    }
    return ret;
}
