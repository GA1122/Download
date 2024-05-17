PP_Bool HandleEvent(PP_Resource resource, const PP_InputEvent* event) {
  scoped_refptr<PPB_Widget_Impl> widget(
      Resource::GetAs<PPB_Widget_Impl>(resource));
  return BoolToPPBool(widget && widget->HandleEvent(event));
}
