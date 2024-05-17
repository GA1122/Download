void DevToolsWindow::ColorPickedInEyeDropper(int r, int g, int b, int a) {
  base::DictionaryValue color;
  color.SetInteger("r", r);
  color.SetInteger("g", g);
  color.SetInteger("b", b);
  color.SetInteger("a", a);
  bindings_->CallClientFunction("DevToolsAPI.eyeDropperPickedColor", &color,
                                nullptr, nullptr);
}
