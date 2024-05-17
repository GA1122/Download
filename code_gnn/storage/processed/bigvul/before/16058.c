size_t xmp_string_len(XmpStringPtr s)
{
    CHECK_PTR(s, 0);
    return reinterpret_cast<const std::string *>(s)->size();
}
