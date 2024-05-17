void RenderWidgetHostViewAura::ReleaseSoftwareFrame(
    uint32 output_surface_id,
    unsigned software_frame_id) {
  SendReclaimSoftwareFrames();
  DCHECK(!released_software_frame_);
  released_software_frame_.reset(new ReleasedFrameInfo(
      output_surface_id, software_frame_id));
}
