BarProp* LocalDOMWindow::menubar() const {
  if (!menubar_)
    menubar_ = BarProp::Create(GetFrame(), BarProp::kMenubar);
  return menubar_.Get();
}
