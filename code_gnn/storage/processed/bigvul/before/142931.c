void HTMLMediaElement::TogglePlayState() {
  if (paused())
    Play();
  else
    pause();
}
