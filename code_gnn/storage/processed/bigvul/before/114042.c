  virtual bool GetCellColors(int model_row,
                             int column,
                             ItemColor* foreground,
                             ItemColor* background,
                             LOGFONT* logfont) {
    if (!model_->IsBackgroundResource(model_row))
      return false;

    background->color_is_set = true;
    background->color = kBackgroundResourceHighlight;
    foreground->color_is_set = false;
    return true;
  }
