XmpPtr xmp_files_get_new_xmp(XmpFilePtr xf)
{
    CHECK_PTR(xf, NULL);
    RESET_ERROR;
    auto txf = reinterpret_cast<SXMPFiles *>(xf);

    bool result = false;
    try {
        auto xmp = std::unique_ptr<SXMPMeta>(new SXMPMeta);
        result = txf->GetXMP(xmp.get());
        if (result) {
            return reinterpret_cast<XmpPtr>(xmp.release());
        }
    }
    catch (const XMP_Error &e) {
        set_error(e);
    }
    return NULL;
}
