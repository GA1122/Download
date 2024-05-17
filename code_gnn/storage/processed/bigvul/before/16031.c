bool xmp_get_property_date(XmpPtr xmp, const char *schema, const char *name,
                           XmpDateTime *property, uint32_t *propsBits)
{
    CHECK_PTR(xmp, false);
    RESET_ERROR;

    bool ret = false;
    try {
        auto txmp = reinterpret_cast<const SXMPMeta *>(xmp);
        XMP_OptionBits optionBits;
        XMP_DateTime dt;
        ret = txmp->GetProperty_Date(schema, name, &dt, &optionBits);
        ASSIGN((*property), dt);
        if (propsBits) {
            *propsBits = optionBits;
        }
    }
    catch (const XMP_Error &e) {
        set_error(e);
    }
    return ret;
}
