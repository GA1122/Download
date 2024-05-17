  DocumentsProviderTestVolume(
      const std::string& name,
      arc::FakeFileSystemInstance* const file_system_instance,
      const std::string& authority,
      const std::string& root_document_id)
      : TestVolume(name),
        file_system_instance_(file_system_instance),
        authority_(authority),
        root_document_id_(root_document_id) {}
