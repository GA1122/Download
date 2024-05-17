bool xmp_set_property_bool(XmpPtr xmp, const char *schema, const char *name,
                           bool value, uint32_t optionBits)
{
    CHECK_PTR(xmp, false);
    RESET_ERROR;

    bool ret = false;
    auto txmp = reinterpret_cast<SXMPMeta *>(xmp);
    try {
        txmp->SetProperty_Bool(schema, name, value, optionBits);
        ret = true;
    }
    catch (const XMP_Error &e) {
        set_error(e);
    }
    catch (...) {
    }
    return ret;
}
