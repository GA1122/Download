PP_Bool IsWidget(PP_Resource resource) {
  return BoolToPPBool(!!Resource::GetAs<PPB_Widget_Impl>(resource));
}
