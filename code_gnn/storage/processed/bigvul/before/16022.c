bool xmp_files_put_xmp(XmpFilePtr xf, XmpPtr xmp)
{
    CHECK_PTR(xf, false);
    CHECK_PTR(xmp, false);
    RESET_ERROR;
    auto txf = reinterpret_cast<SXMPFiles *>(xf);

    try {
        txf->PutXMP(*reinterpret_cast<const SXMPMeta *>(xmp));
    }
    catch (const XMP_Error &e) {
        set_error(e);
        return false;
    }
    return true;
}
