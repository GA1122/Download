void ShellContentUtilityClient::RegisterServices(StaticServiceMap* services) {
  {
    service_manager::EmbeddedServiceInfo info;
    info.factory = base::BindRepeating(&CreateTestService);
    services->insert(std::make_pair(kTestServiceUrl, info));
  }

  {
    service_manager::EmbeddedServiceInfo info;
    info.factory = base::BindRepeating(&echo::CreateEchoService);
    services->insert(std::make_pair(echo::mojom::kServiceName, info));
  }
}
