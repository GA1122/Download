WebContentsImpl::ColorChooserInfo::ColorChooserInfo(int render_process_id,
                                                    int render_frame_id,
                                                    ColorChooser* chooser,
                                                    int identifier)
    : render_process_id(render_process_id),
      render_frame_id(render_frame_id),
      chooser(chooser),
      identifier(identifier) {
}
