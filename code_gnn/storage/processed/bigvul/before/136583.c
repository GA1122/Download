  sk_sp<PaintRecord> MakeRecord(const FloatRect& rect, Color color) {
    rect_ = rect;
    PaintRecorder recorder;
    PaintCanvas* canvas = recorder.beginRecording(rect);
    PaintFlags flags;
    flags.setColor(color.Rgb());
    canvas->drawRect(rect, flags);
    return recorder.finishRecordingAsPicture();
  }
