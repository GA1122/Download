void HTMLMediaElement::FlingingStopped() {
  if (GetWebMediaPlayer())
    GetWebMediaPlayer()->FlingingStopped();
}
