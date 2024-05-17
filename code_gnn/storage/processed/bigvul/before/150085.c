  void CallOnDraw() {
    if (!TestEnded()) {
      bool resourceless_software_draw = false;
      ImplThreadTaskRunner()->PostTask(
          FROM_HERE, base::Bind(&OnDrawCompositorFrameSink::OnDraw,
                                base::Unretained(compositor_frame_sink_),
                                resourceless_software_draw));
    }
  }
