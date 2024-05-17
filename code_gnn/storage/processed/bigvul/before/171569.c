String8 String8::getPathDir(void) const
{
 const char* cp;
 const char*const str = mString;

    cp = strrchr(str, OS_PATH_SEPARATOR);
 if (cp == NULL)
 return String8("");
 else
 return String8(str, cp - str);
}
