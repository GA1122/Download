void Launcher::UpdateBackground(int alpha) {
  ui::Layer* layer = widget_->GetNativeView()->layer();
  layer->SetColor(SkColorSetARGB(alpha, 0, 0, 0));
}
