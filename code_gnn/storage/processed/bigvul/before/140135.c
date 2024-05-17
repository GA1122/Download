void HTMLMediaElement::togglePlayState() {
  if (paused())
    play();
  else
    pause();
}
