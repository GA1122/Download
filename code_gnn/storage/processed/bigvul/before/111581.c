    virtual void GetFileCallback(base::PlatformFileError error,
                                 const FilePath& file_path,
                                 const std::string& mime_type,
                                 GDataFileType file_type) {
      last_error_ = error;
      download_path_ = file_path;
      mime_type_ = mime_type;
      file_type_ = file_type;
    }
