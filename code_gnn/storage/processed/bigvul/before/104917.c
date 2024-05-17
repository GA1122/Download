void SetLocation(PP_Resource resource, const PP_Rect* location) {
  scoped_refptr<PPB_Widget_Impl> widget(
      Resource::GetAs<PPB_Widget_Impl>(resource));
  if (widget)
    widget->SetLocation(location);
}
