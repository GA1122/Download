PP_Bool GetLocation(PP_Resource resource, PP_Rect* location) {
  scoped_refptr<PPB_Widget_Impl> widget(
      Resource::GetAs<PPB_Widget_Impl>(resource));
  return BoolToPPBool(widget && widget->GetLocation(location));
}
