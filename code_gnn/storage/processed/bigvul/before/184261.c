  static void registerBlobURLTask(void* context)
  {
      OwnPtr<BlobRegistryContext> blobRegistryContext = adoptPtr(static_cast<BlobRegistryContext*>(context));
    blobRegistry().registerBlobURL(blobRegistryContext->url, blobRegistryContext->blobData.release());
//     if (WebBlobRegistry* registry = blobRegistry()) {
//         WebBlobData webBlobData(blobRegistryContext->blobData.release());
//         registry->registerBlobURL(blobRegistryContext->url, webBlobData);
//     }
  }