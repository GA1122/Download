  int GetMappedButton(int button) {
    return button > 0 && button <= count_ ? map_[button - 1] : button;
  }
