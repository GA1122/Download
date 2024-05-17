bool xmp_files_free(XmpFilePtr xf)
{
    CHECK_PTR(xf, false);
    RESET_ERROR;
    auto txf = reinterpret_cast<SXMPFiles *>(xf);
    try {
        delete txf;
    }
    catch (const XMP_Error &e) {
        set_error(e);
        return false;
    }
    return true;
}
