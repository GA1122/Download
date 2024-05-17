bool xmp_set_localized_text(XmpPtr xmp, const char *schema, const char *name,
                            const char *genericLang, const char *specificLang,
                            const char *value, uint32_t optionBits)
{
    CHECK_PTR(xmp, false);
    RESET_ERROR;

    bool ret = true;
    auto txmp = reinterpret_cast<SXMPMeta *>(xmp);
    try {
        txmp->SetLocalizedText(schema, name, genericLang, specificLang, value,
                               optionBits);
    }
    catch (const XMP_Error &e) {
        set_error(e);
        ret = false;
    }
    catch (...) {
        ret = false;
    }
    return ret;
}
