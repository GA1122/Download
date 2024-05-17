inline void Splash::drawPixel(SplashPipe *pipe, int x, int y, GBool noClip) {
  if (unlikely(y < 0))
    return;

  if (noClip || state->clip->test(x, y)) {
    pipeSetXY(pipe, x, y);
    (this->*pipe->run)(pipe);
    updateModX(x);
    updateModY(y);
  }
}
