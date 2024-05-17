void PluginChannel::OnCreateInstance(const std::string& mime_type,
                                     int* instance_id) {
  *instance_id = GenerateRouteID();
  scoped_refptr<WebPluginDelegateStub> stub(new WebPluginDelegateStub(
      mime_type, *instance_id, this));
  AddRoute(*instance_id, stub, NULL);
  plugin_stubs_.push_back(stub);
}
