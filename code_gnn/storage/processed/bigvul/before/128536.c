void Instance::CreateHorizontalScrollbar() {
  if (h_scrollbar_.get())
    return;

  h_scrollbar_.reset(new pp::Scrollbar_Dev(this, false));
}
