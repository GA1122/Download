void xmp_string_free(XmpStringPtr s)
{
    auto str = reinterpret_cast<std::string *>(s);
    delete str;
}
