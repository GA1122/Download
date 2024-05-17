void Tab::SetClosing(bool closing) {
  closing_ = closing;
  ActiveStateChanged();

  if (closing) {
    focus_ring_.reset();
   }
 }
