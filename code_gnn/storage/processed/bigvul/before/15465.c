bool Tar::FillHeader(wxString &mFilePath, const wxString& dmod_folder, char *header512, fileinfo *finfo)
{
  if (!S_ISREG(finfo->mode))
    return false;

  char* ptr = header512;

  strncpy(ptr, (dmod_folder.Lower() + _T("/") + mFilePath.Lower()).mb_str(wxConvUTF8), 100);
  ptr += 100;
  
  strncpy(ptr, "0100644", 8);
  ptr += 8;
  strncpy(ptr, "0000000", 8);
  ptr += 8;
  strncpy(ptr, "0000000", 8);
  ptr += 8;
 
  sprintf(ptr, "%011o", finfo->size);
  ptr += 12;
  
  sprintf(ptr, "%011o", finfo->mtime);
  ptr += 12;

  memset(ptr, ' ', 8);
  ptr += 8;

  *ptr = '0';  
  ptr++;
  strncpy(ptr, "", 100);
  ptr += 100;

  strncpy(ptr, "ustar  ", 8);
  ptr += 8;

  strncpy(ptr, "root", 32);
  ptr += 32;
  strncpy(ptr, "root", 32);
  ptr += 32;

  strncpy(ptr, "", 8);
  ptr += 8;
  strncpy(ptr, "", 8);
  ptr += 8;

  strncpy(ptr, "", 167);


  unsigned char* ptru = NULL;  
  int iChksum = 0;
  for (ptru = (unsigned char*) header512; ptru < (unsigned char*)(header512 + 512); ptru++)
    iChksum += *ptru;
  
  ptr = header512 + 148;
  sprintf(ptr, "%07o", iChksum);

  return true;
}
