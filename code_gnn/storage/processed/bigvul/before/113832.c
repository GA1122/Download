bool InitTargetServices(sandbox::TargetServices* target_services) {
  DCHECK(target_services);
  DCHECK(!g_target_services);
  sandbox::ResultCode result = target_services->Init();
  g_target_services = target_services;
  return SBOX_ALL_OK == result;
}
