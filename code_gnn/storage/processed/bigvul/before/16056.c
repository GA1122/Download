const char *xmp_string_cstr(XmpStringPtr s)
{
    CHECK_PTR(s, NULL);
    return reinterpret_cast<const std::string *>(s)->c_str();
}
