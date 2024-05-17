  StartFileUploadParams(const FilePath& in_local_file_path,
                        const FilePath& in_remote_file_path,
                        const FileOperationCallback& in_callback)
      : local_file_path(in_local_file_path),
        remote_file_path(in_remote_file_path),
        callback(in_callback) {}
