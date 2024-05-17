bool Tar::CreateReal(const wxString& dmod_folder, double *compression_ratio, wxProgressDialog* aProgressDialog)
{
  bool aborted = false;
  wxArrayString wxasFileList;
  aProgressDialog->Update(0, _("Listing files..."));
  IOUtils::GetAllDModFiles(strCompressDir, wxasFileList);
  
  int iNumEntries = wxasFileList.GetCount();

  fileinfo *fileinfos = new fileinfo[iNumEntries];
  int total_file_data = 0;
  for (unsigned int i = 0; i < wxasFileList.GetCount(); ++i)
    {
      fileinfos[i].fullpath = strCompressDir + _T("/") + wxasFileList.Item(i);
#if defined _WIN32 || defined __WIN32__ || defined __CYGWIN__
      struct _stat sb;
      if (_wstat(fileinfos[i].fullpath.fn_str(), &sb) < 0)
#else
      struct stat sb;
      if (stat(fileinfos[i].fullpath.fn_str(), &sb) < 0)
#endif
	{
	  perror("stat");
	  fileinfos[i].mode = 0;  
	  continue;
	}
      fileinfos[i].mode = sb.st_mode;
      fileinfos[i].size = sb.st_size;
      fileinfos[i].mtime = sb.st_mtime;

       
#ifdef HAVE_STRUCT_STAT_ST_BLKSIZE
      fileinfos[i].blksize = sb.st_blksize;
#else
      fileinfos[i].blksize = 8192;
#endif

      total_file_data += sb.st_size;
    }


  aProgressDialog->Update(0, _("Initializing..."));
  FILE* out = fopen(mFilePath.fn_str(), "wb");
  if (!out)
    {
      wxLogError(_("Error: Could not open tar file '%s' for bzip compression."), mFilePath.c_str());
      return false;
    }
  
   
  BZFILE* bz_out = 0;
  int iErr = 0;
  int compress_factor = 9;
  int debug_verbosity = 0;
  int workFactor = 0;
  bz_out = BZ2_bzWriteOpen(&iErr, out,
			   compress_factor,
			   debug_verbosity,
			   workFactor);
  
  if (iErr != BZ_OK)
    {
      wxLogError(_("Error: Could not initialize compression method!"
		     "  "
		     "Will not generate a correct .dmod file."
		     "  "
		     "Quitting."));
      fclose(out);
      return false;
    }
  

  int total_file_data_written = 0;
  for (unsigned int i = 0; i < wxasFileList.GetCount(); ++i)
    {
      if (aProgressDialog != NULL)
        {
	  int lPercent = 98 * (long long)total_file_data_written / total_file_data;
	  aProgressDialog->Update(lPercent, wxasFileList.Item(i));
        }
      
      char header[512];
      if (!FillHeader(wxasFileList.Item(i), dmod_folder, header, &fileinfos[i]))
	continue;
      BZ2_bzWrite(&iErr, bz_out, header, 512);
      
      FILE *in = fopen(fileinfos[i].fullpath.fn_str(), "rb");
      if (in == NULL)
        {
	  wxLogFatalError(_("Error: File '%s' not found!  Cannot archive file."),
			    fileinfos[i].fullpath.c_str());
	  throw;
        }
  

      int ebufsiz = fileinfos[i].blksize;  
      unsigned char* szBuf = (unsigned char*)malloc(ebufsiz);
      int cur_file_nb_written = 0;

      while(!feof(in))
        {
	  int nb_read = fread(szBuf, 1, ebufsiz, in);
	  BZ2_bzWrite(&iErr, bz_out, szBuf, nb_read);
	  cur_file_nb_written += nb_read;
	  total_file_data_written += nb_read;
	  if (aProgressDialog != NULL)
	    {
	      int lPercent = 98 * (long long)total_file_data_written / total_file_data;
	      bool cont = aProgressDialog->Update(lPercent);
	      if (!cont)
		{
		  aborted = true;
		  fclose(in);
		  free(szBuf);
		  goto clean_up;
		}
	    }
        }
      fclose(in);

      int iAmountToRound = (512 - (cur_file_nb_written % (512))) % 512;
      memset(szBuf, 0, iAmountToRound);
      BZ2_bzWrite(&iErr, bz_out, szBuf, iAmountToRound);
      
      free(szBuf);
    }	
  

  char eoa[2*512];
  memset(eoa, 0, 2*512);
  BZ2_bzWrite(&iErr, bz_out, eoa, 2*512);
  
 clean_up:
  aProgressDialog->Update(98, _("Closing..."));
  int force_flush = 0;  
  unsigned int nbytes_in = 0;
  unsigned int nbytes_out = 0;
  BZ2_bzWriteClose(&iErr, bz_out, force_flush, &nbytes_in, &nbytes_out);
  *compression_ratio = (double) nbytes_in / nbytes_out;
  fclose(out);
  delete[] fileinfos;
  
  aProgressDialog->Update(100);  
  return !aborted;
}
