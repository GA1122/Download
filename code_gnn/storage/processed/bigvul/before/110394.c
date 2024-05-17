void PluginModule::SetReserveInstanceIDCallback(
    PP_Bool (*reserve)(PP_Module, PP_Instance)) {
  DCHECK(!reserve_instance_id_) << "Only expect one set.";
  reserve_instance_id_ = reserve;
}
