  MediaViewTestVolume(arc::FakeFileSystemInstance* const file_system_instance,
                      const std::string& authority,
                      const std::string& root_document_id)
      : DocumentsProviderTestVolume(root_document_id,
                                    file_system_instance,
                                    authority,
                                    root_document_id) {}
