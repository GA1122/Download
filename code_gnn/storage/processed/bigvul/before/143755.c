PP_Resource PPB_Flash_MessageLoop_Impl::Create(PP_Instance instance) {
  return (new PPB_Flash_MessageLoop_Impl(instance))->GetReference();
}
