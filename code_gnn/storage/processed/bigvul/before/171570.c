String8 String8::getPathExtension(void) const
{
 char* ext;

    ext = find_extension();
 if (ext != NULL)
 return String8(ext);
 else
 return String8("");
}
