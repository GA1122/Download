BinaryUploadService::BinaryUploadService(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
    Profile* profile)
    : url_loader_factory_(url_loader_factory),
      binary_fcm_service_(BinaryFCMService::Create(profile)),
      weakptr_factory_(this) {}
