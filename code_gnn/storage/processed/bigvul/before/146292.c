static String ExtractWebGLContextCreationError(
    const Platform::GraphicsInfo& info) {
  StringBuilder builder;
  builder.Append("Could not create a WebGL context");
  FormatWebGLStatusString(
      "VENDOR",
      info.vendor_id ? String::Format("0x%04x", info.vendor_id) : "0xffff",
      builder);
  FormatWebGLStatusString(
      "DEVICE",
      info.device_id ? String::Format("0x%04x", info.device_id) : "0xffff",
      builder);
  FormatWebGLStatusString("GL_VENDOR", info.vendor_info, builder);
  FormatWebGLStatusString("GL_RENDERER", info.renderer_info, builder);
  FormatWebGLStatusString("GL_VERSION", info.driver_version, builder);
  FormatWebGLStatusString("Sandboxed", info.sandboxed ? "yes" : "no", builder);
  FormatWebGLStatusString("Optimus", info.optimus ? "yes" : "no", builder);
  FormatWebGLStatusString("AMD switchable", info.amd_switchable ? "yes" : "no",
                          builder);
  FormatWebGLStatusString(
      "Reset notification strategy",
      String::Format("0x%04x", info.reset_notification_strategy).Utf8().data(),
      builder);
  FormatWebGLStatusString("GPU process crash count",
                          String::Number(info.process_crash_count), builder);
  FormatWebGLStatusString("ErrorMessage", info.error_message.Utf8().data(),
                          builder);
  builder.Append('.');
  return builder.ToString();
}
