UtilityServiceFactory::UtilityServiceFactory()
    : network_registry_(base::MakeUnique<service_manager::BinderRegistry>()) {}
