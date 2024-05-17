 virtual ~ExternalFrameBufferMD5Test() {
 if (md5_file_ != NULL)
      fclose(md5_file_);
 }
