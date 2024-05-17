  void RegisterRoot() {
    arc::FakeFileSystemInstance::Document document(
        authority_, root_document_id_, "", "", arc::kAndroidDirectoryMimeType,
        0, 0);
    file_system_instance_->AddDocument(document);
  }
