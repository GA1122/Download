bool HTMLMediaElement::supportsSave() const {
  return webMediaPlayer() && webMediaPlayer()->supportsSave();
}
