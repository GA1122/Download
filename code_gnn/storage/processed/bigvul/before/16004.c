bool xmp_append_array_item(XmpPtr xmp, const char *schema, const char *name,
                           uint32_t arrayOptions, const char *value,
                           uint32_t optionBits)
{
    CHECK_PTR(xmp, false);
    RESET_ERROR;

    bool ret = false;
    auto txmp = reinterpret_cast<SXMPMeta *>(xmp);
    try {
        txmp->AppendArrayItem(schema, name, arrayOptions, value, optionBits);
        ret = true;
    }
    catch (const XMP_Error &e) {
        set_error(e);
    }
    catch (...) {
    }
    return ret;
}
