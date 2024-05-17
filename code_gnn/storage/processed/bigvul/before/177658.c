 virtual void Next() {
 ++frame_;
 SetSize(ScaleForFrameNumber(frame_, kInitialWidth),
 ScaleForFrameNumber(frame_, kInitialHeight));
 FillFrame();
 }
