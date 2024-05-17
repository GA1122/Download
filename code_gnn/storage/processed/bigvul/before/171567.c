String8 String8::getBasePath(void) const
{
 char* ext;
 const char* const str = mString;

    ext = find_extension();
 if (ext == NULL)
 return String8(*this);
 else
 return String8(str, ext - str);
}
