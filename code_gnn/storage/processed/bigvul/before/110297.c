 const PPB_UMA_Private* GetUMAInterface() {
   pp::Module *module = pp::Module::Get();
   CHECK(module);
  return static_cast<const PPB_UMA_Private*>(
      module->GetBrowserInterface(PPB_UMA_PRIVATE_INTERFACE));
}
