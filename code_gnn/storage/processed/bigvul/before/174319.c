static const char* getReturnString(size_t idx)
{
 if (idx < sizeof(kReturnStrings) / sizeof(kReturnStrings[0]))
 return kReturnStrings[idx];
 else
 return "unknown";
}
