bool xmp_files_close(XmpFilePtr xf, XmpCloseFileOptions options)
{
    CHECK_PTR(xf, false);
    RESET_ERROR;
    try {
        auto txf = reinterpret_cast<SXMPFiles *>(xf);
        txf->CloseFile(options);
    }
    catch (const XMP_Error &e) {
        set_error(e);
        return false;
    }
    return true;
}
