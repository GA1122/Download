    ExecuteSelectParams(Type type,
                        const std::wstring& title,
                        const FilePath& default_path,
                        const FileTypeInfo* file_types,
                        int file_type_index,
                        const std::wstring& default_extension,
                        RunState run_state,
                        HWND owner,
                        void* params)
        : type(type),
          title(title),
          default_path(default_path),
          file_type_index(file_type_index),
          default_extension(default_extension),
          run_state(run_state),
          owner(owner),
          params(params) {
      if (file_types) {
        this->file_types = *file_types;
      } else {
        this->file_types.include_all_files = true;
      }
    }
