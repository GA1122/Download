Tar::Tar(wxString &szFile, wxString &szCompressDir) : 
  DFile(szFile)
{
  strCompressDir = szCompressDir;
  bCanCompress = true;
}
