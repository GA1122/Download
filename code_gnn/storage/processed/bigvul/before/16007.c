bool xmp_delete_localized_text(XmpPtr xmp, const char *schema, const char *name,
                               const char *genericLang,
                               const char *specificLang)
{
    CHECK_PTR(xmp, false);
    RESET_ERROR;

    bool ret = true;
    auto txmp = reinterpret_cast<SXMPMeta *>(xmp);
    try {
        txmp->DeleteLocalizedText(schema, name, genericLang, specificLang);
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