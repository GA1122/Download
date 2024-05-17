    virtual void GetFileInfoCallback(
        base::PlatformFileError error,
        scoped_ptr<GDataFileProto> file_proto) {
      last_error_ = error;
      file_proto_ = file_proto.Pass();
    }
