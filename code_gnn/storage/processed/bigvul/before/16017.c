bool xmp_files_get_xmp(XmpFilePtr xf, XmpPtr xmp)
{
    CHECK_PTR(xf, false);
    CHECK_PTR(xmp, false);
    RESET_ERROR;
    bool result = false;
    try {
        auto txf = reinterpret_cast<SXMPFiles *>(xf);

        result = txf->GetXMP(reinterpret_cast<SXMPMeta *>(xmp));
    }
    catch (const XMP_Error &e) {
        set_error(e);
    }
    return result;
}
