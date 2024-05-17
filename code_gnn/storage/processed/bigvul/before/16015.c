bool xmp_files_get_format_info(XmpFileType format,
                               XmpFileFormatOptions *options)
{
    RESET_ERROR;

    bool result = false;
    try {
        result = SXMPFiles::GetFormatInfo(format, (XMP_OptionBits *)options);
    }
    catch (const XMP_Error &e) {
        set_error(e);
        return false;
    }
    return result;
}
