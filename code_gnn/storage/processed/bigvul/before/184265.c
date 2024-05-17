  static void unregisterBlobURLTask(void* context)
  {
      OwnPtr<BlobRegistryContext> blobRegistryContext = adoptPtr(static_cast<BlobRegistryContext*>(context));
    blobRegistry().unregisterBlobURL(blobRegistryContext->url);
//     if (WebBlobRegistry* registry = blobRegistry())
//         registry->unregisterBlobURL(blobRegistryContext->url);
  }