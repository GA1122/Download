  virtual ~TabContextMenuContents() {
    if (controller_)
      controller_->tabstrip_->StopAllHighlighting();
  }
