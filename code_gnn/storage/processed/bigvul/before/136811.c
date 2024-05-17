BarProp* LocalDOMWindow::locationbar() const {
  if (!locationbar_)
    locationbar_ = BarProp::Create(GetFrame(), BarProp::kLocationbar);
  return locationbar_.Get();
}
