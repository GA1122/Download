    TestEntryInfo(EntryType type,
                  const std::string& source_file_name,
                  const std::string& target_path)
        : type(type),
          shared_option(NONE),
          source_file_name(source_file_name),
          target_path(target_path),
          last_modified_time(base::Time::Now()) {}
