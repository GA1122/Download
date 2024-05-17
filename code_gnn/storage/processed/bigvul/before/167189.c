void HTMLMediaElement::exitPictureInPicture(
    WebMediaPlayer::PipWindowClosedCallback callback) {
  if (GetWebMediaPlayer())
    GetWebMediaPlayer()->ExitPictureInPicture(std::move(callback));
}
