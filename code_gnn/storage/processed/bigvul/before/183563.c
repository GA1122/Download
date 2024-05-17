 NPObject* WebPluginProxy::GetPluginElement() {
   if (plugin_element_)
     return WebBindings::retainObject(plugin_element_);
  
    int npobject_route_id = channel_->GenerateRouteID();
    bool success = false;
  intptr_t npobject_ptr = NULL;
  Send(new PluginHostMsg_GetPluginElement(
      route_id_, npobject_route_id, &success, &npobject_ptr));
//   Send(new PluginHostMsg_GetPluginElement(route_id_, npobject_route_id, &success));
    if (!success)
      return NULL;
  
   plugin_element_ = NPObjectProxy::Create(
       channel_, npobject_route_id, containing_window_, page_url_);
 
   return plugin_element_;
 }