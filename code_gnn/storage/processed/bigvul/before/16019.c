XmpFilePtr xmp_files_new()
{
    RESET_ERROR;

    try {
        auto txf = std::unique_ptr<SXMPFiles>(new SXMPFiles());

        return reinterpret_cast<XmpFilePtr>(txf.release());
    }
    catch (const XMP_Error &e) {
        set_error(e);
    }
    return NULL;
}
