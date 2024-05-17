LocalDOMWindow* Document::ExecutingWindow() const {
  if (LocalDOMWindow* owning_window = domWindow())
    return owning_window;
  if (HTMLImportsController* import = this->ImportsController())
    return import->Master()->domWindow();
  return 0;
}
