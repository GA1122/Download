bool xmp_set_property_date(XmpPtr xmp, const char *schema, const char *name,
                           const XmpDateTime *value, uint32_t optionBits)
{
    CHECK_PTR(xmp, false);
    RESET_ERROR;

    bool ret = false;
    auto txmp = reinterpret_cast<SXMPMeta *>(xmp);
    try {
        XMP_DateTime dt;
        ASSIGN(dt, (*value));
        txmp->SetProperty_Date(schema, name, dt, optionBits);
        ret = true;
    }
    catch (const XMP_Error &e) {
        set_error(e);
    }
    catch (...) {
    }
    return ret;
}
