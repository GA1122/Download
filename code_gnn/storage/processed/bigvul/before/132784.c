void PepperVideoRenderer3D::OnPaintDone(int32_t result) {
  CHECK_EQ(result, PP_OK) << "Graphics3D::SwapBuffers() failed";

  paint_pending_ = false;
  base::TimeDelta paint_time =
      base::TimeTicks::Now() - latest_paint_started_time_;
  stats_.video_paint_ms()->Record(paint_time.InMilliseconds());

  PaintIfNeeded();
}
