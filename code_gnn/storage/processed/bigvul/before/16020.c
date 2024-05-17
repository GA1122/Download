bool xmp_files_open(XmpFilePtr xf, const char *path, XmpOpenFileOptions options)
{
    CHECK_PTR(xf, false);
    RESET_ERROR;
    auto txf = reinterpret_cast<SXMPFiles *>(xf);
    try {
        return txf->OpenFile(path, XMP_FT_UNKNOWN, options);
    }
    catch (const XMP_Error &e) {
        set_error(e);
    }
    return false;
}
