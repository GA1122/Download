bool xmp_free(XmpPtr xmp)
{
    CHECK_PTR(xmp, false);
    RESET_ERROR;
    SXMPMeta *txmp = (SXMPMeta *)xmp;
    delete txmp;
    return true;
}
