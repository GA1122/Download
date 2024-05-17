bool Tar::Create(const wxString& dmod_folder, double *compression_ratio, wxProgressDialog* aProgressDialog)
{
  if (!bCanCompress)
    return wxEmptyString;

  wxString strCwd = ::wxGetCwd();

  ::wxSetWorkingDirectory(strCompressDir);
  bool result = CreateReal(dmod_folder, compression_ratio, aProgressDialog);

  ::wxSetWorkingDirectory(strCwd);

  return result;
}
