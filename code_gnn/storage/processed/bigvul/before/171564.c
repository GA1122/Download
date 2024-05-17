char* String8::find_extension(void) const
{
 const char* lastSlash;
 const char* lastDot;
 const char* const str = mString;

    lastSlash = strrchr(str, OS_PATH_SEPARATOR);
 if (lastSlash == NULL)
        lastSlash = str;
 else
        lastSlash++;

    lastDot = strrchr(lastSlash, '.');
 if (lastDot == NULL)
 return NULL;

 return const_cast<char*>(lastDot);
}
