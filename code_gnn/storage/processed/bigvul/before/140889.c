  BlobLoader(PassRefPtr<BlobDataHandle> blobDataHandle,
             PresentationConnection* PresentationConnection)
      : m_PresentationConnection(PresentationConnection),
        m_loader(FileReaderLoader::create(FileReaderLoader::ReadAsArrayBuffer,
                                          this)) {
    m_loader->start(m_PresentationConnection->getExecutionContext(),
                    std::move(blobDataHandle));
  }
