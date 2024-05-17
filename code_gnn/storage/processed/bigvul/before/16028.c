bool xmp_get_localized_text(XmpPtr xmp, const char *schema, const char *name,
                            const char *genericLang, const char *specificLang,
                            XmpStringPtr actualLang, XmpStringPtr itemValue,
                            uint32_t *propsBits)
{
    CHECK_PTR(xmp, false);
    RESET_ERROR;

    bool ret = false;
    try {
        auto txmp = reinterpret_cast<const SXMPMeta *>(xmp);
        XMP_OptionBits optionBits;
        ret = txmp->GetLocalizedText(schema, name, genericLang, specificLang,
                                     STRING(actualLang), STRING(itemValue),
                                     &optionBits);
        if (propsBits) {
            *propsBits = optionBits;
        }
    }
    catch (const XMP_Error &e) {
        set_error(e);
        ret = false;
    }
    return ret;
}
