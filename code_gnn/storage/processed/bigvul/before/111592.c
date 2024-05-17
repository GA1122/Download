    virtual void ReadDirectoryCallback(
        base::PlatformFileError error,
        scoped_ptr<GDataDirectoryProto> directory_proto) {
      last_error_ = error;
      directory_proto_ = directory_proto.Pass();
    }
