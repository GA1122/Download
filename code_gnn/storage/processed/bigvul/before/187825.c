 int Track::Info::CopyStr(char* Info::*str, Info& dst_) const {
  if (str == static_cast<char * Info::*>(NULL))
  return -1;
 
  char*& dst = dst_.*str;
 
  if (dst)  
  return -1;
 
  const char* const src = this->*str;
 
  if (src == NULL)
  return 0;
 
  
    const size_t len = strlen(src);
  
  dst = new (std::nothrow) char[len + 1];
//   dst = SafeArrayAlloc<char>(1, len + 1);
  
    if (dst == NULL)
      return -1;
 
   strcpy(dst, src);
 
  return 0;
 }