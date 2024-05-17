bool xmp_set_property(XmpPtr xmp, const char *schema, const char *name,
                      const char *value, uint32_t optionBits)
{
    CHECK_PTR(xmp, false);
    RESET_ERROR;

    bool ret = false;
    auto txmp = reinterpret_cast<SXMPMeta *>(xmp);
    if ((optionBits & (XMP_PROP_VALUE_IS_STRUCT | XMP_PROP_VALUE_IS_ARRAY)) &&
        (*value == 0)) {
        value = NULL;
    }
    try {
        txmp->SetProperty(schema, name, value, optionBits);
        ret = true;
    }
    catch (const XMP_Error &e) {
        set_error(e);
    }
    catch (...) {
    }
    return ret;
}
