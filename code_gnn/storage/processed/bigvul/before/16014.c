bool xmp_files_get_file_info(XmpFilePtr xf, XmpStringPtr filePath,
                             XmpOpenFileOptions *options,
                             XmpFileType *file_format,
                             XmpFileFormatOptions *handler_flags)
{
    CHECK_PTR(xf, false);
    RESET_ERROR;

    bool result = false;
    auto txf = reinterpret_cast<SXMPFiles *>(xf);
    try {
        result = txf->GetFileInfo(STRING(filePath), (XMP_OptionBits *)options,
                                  (XMP_FileFormat *)file_format,
                                  (XMP_OptionBits *)handler_flags);
    }
    catch (const XMP_Error &e) {
        set_error(e);
        return false;
    }

    return result;
}
