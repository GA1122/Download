 virtual void EndPassHook() {
#if WRITE_COMPRESSED_STREAM
 if (outfile_) {
 if (!fseek(outfile_, 0, SEEK_SET))
        write_ivf_file_header(&cfg_, out_frames_, outfile_);
      fclose(outfile_);
      outfile_ = NULL;
 }
#endif
 }
