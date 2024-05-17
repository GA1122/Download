  static void finalizeStreamTask(void* context)
  {
      OwnPtr<BlobRegistryContext> blobRegistryContext = adoptPtr(static_cast<BlobRegistryContext*>(context));
    blobRegistry().finalizeStream(blobRegistryContext->url);
//     if (WebBlobRegistry* registry = blobRegistry())
//         registry->finalizeStream(blobRegistryContext->url);
  }