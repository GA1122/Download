HeadlessWebContents::Builder::MojoService::MojoService(
    const std::string& service_name,
    const ServiceFactoryCallback& service_factory)
    : service_name(service_name), service_factory(service_factory) {}
