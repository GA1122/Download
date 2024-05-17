void BlinkTestRunner::CaptureDump() {
  WebTestInterfaces* interfaces =
      LayoutTestRenderProcessObserver::GetInstance()->test_interfaces();
  TRACE_EVENT0("shell", "BlinkTestRunner::CaptureDump");

  if (interfaces->TestRunner()->ShouldDumpAsAudio()) {
    std::vector<unsigned char> vector_data;
    interfaces->TestRunner()->GetAudioData(&vector_data);
    Send(new ShellViewHostMsg_AudioDump(routing_id(), vector_data));
  } else {
    Send(new ShellViewHostMsg_TextDump(routing_id(),
                                       proxy()->CaptureTree(false)));

    if (test_config_.enable_pixel_dumping &&
        interfaces->TestRunner()->ShouldGeneratePixelResults()) {
      CHECK(render_view()->GetWebView()->isAcceleratedCompositingActive());
      proxy()->CapturePixelsAsync(base::Bind(
          &BlinkTestRunner::CaptureDumpPixels, base::Unretained(this)));
      return;
    }
  }

  CaptureDumpComplete();
}
