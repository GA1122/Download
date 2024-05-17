bool xmp_delete_property(XmpPtr xmp, const char *schema, const char *name)
{
    CHECK_PTR(xmp, false);
    RESET_ERROR;

    bool ret = true;
    auto txmp = reinterpret_cast<SXMPMeta *>(xmp);
    try {
        txmp->DeleteProperty(schema, name);
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
