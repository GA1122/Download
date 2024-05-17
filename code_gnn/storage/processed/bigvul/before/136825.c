BarProp* LocalDOMWindow::personalbar() const {
  if (!personalbar_)
    personalbar_ = BarProp::Create(GetFrame(), BarProp::kPersonalbar);
  return personalbar_.Get();
}
