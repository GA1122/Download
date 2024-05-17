  virtual bool Mount(Profile* profile) {
    RegisterRoot();

    VolumeManager::Get(profile)->OnDocumentsProviderRootAdded(
        authority_, root_document_id_, root_document_id_, name(), "", GURL(),
        true, std::vector<std::string>());
    return true;
  }
