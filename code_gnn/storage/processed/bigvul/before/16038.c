bool xmp_iterator_next(XmpIteratorPtr iter, XmpStringPtr schema,
                       XmpStringPtr propName, XmpStringPtr propValue,
                       uint32_t *options)
{
    CHECK_PTR(iter, false);
    RESET_ERROR;
    auto titer = reinterpret_cast<SXMPIterator *>(iter);
    return titer->Next(reinterpret_cast<std::string *>(schema),
                       reinterpret_cast<std::string *>(propName),
                       reinterpret_cast<std::string *>(propValue), options);
}
