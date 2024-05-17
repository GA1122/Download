bool xmp_get_array_item(XmpPtr xmp, const char *schema, const char *name,
                        int32_t index, XmpStringPtr property,
                        uint32_t *propsBits)
{
    CHECK_PTR(xmp, false);
    RESET_ERROR;

    bool ret = false;
    try {
        auto txmp = reinterpret_cast<const SXMPMeta *>(xmp);
        XMP_OptionBits optionBits;
        ret = txmp->GetArrayItem(schema, name, index, STRING(property),
                                 &optionBits);
        if (propsBits) {
            *propsBits = optionBits;
        }
    }
    catch (const XMP_Error &e) {
        set_error(e);
    }
    return ret;
}
