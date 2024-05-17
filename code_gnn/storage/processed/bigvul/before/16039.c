bool xmp_iterator_skip(XmpIteratorPtr iter, XmpIterSkipOptions options)
{
    CHECK_PTR(iter, false);
    RESET_ERROR;
    auto titer = reinterpret_cast<SXMPIterator *>(iter);
    titer->Skip(options);
    return true;
}
