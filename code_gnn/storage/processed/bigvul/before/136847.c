BarProp* LocalDOMWindow::toolbar() const {
  if (!toolbar_)
    toolbar_ = BarProp::Create(GetFrame(), BarProp::kToolbar);
  return toolbar_.Get();
}
