  virtual void CreateEntry(const AddEntriesMessage::TestEntryInfo& entry) {
    arc::FakeFileSystemInstance::Document document(
        authority_, entry.name_text, root_document_id_, entry.name_text,
        GetMimeType(entry), GetFileSize(entry),
        entry.last_modified_time.ToJavaTime());
    file_system_instance_->AddDocument(document);
  }
