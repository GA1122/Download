bool xmp_iterator_free(XmpIteratorPtr iter)
{
    CHECK_PTR(iter, false);
    RESET_ERROR;
    auto titer = reinterpret_cast<SXMPIterator *>(iter);
    delete titer;
    return true;
}
