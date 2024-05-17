void Tab::PaintChildren(const views::PaintInfo& info) {
  ui::ClipRecorder clip_recorder(info.context());
  const float paint_recording_scale = info.paint_recording_scale_x();

  const SkPath clip_path = tab_style()->GetPath(
      TabStyle::PathType::kInteriorClip, paint_recording_scale);

  clip_recorder.ClipPathWithAntiAliasing(clip_path);
  View::PaintChildren(info);
}
