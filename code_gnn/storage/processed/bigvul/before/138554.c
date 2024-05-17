std::unique_ptr<KeyboardUI> KeyboardUI::Create() {
  return base::WrapUnique(new KeyboardUIImpl);
}
