void DevToolsWindow::SetEyeDropperActive(bool active) {
  WebContents* web_contents = GetInspectedWebContents();
  if (!web_contents)
    return;
  if (active) {
    eye_dropper_.reset(new DevToolsEyeDropper(
        web_contents, base::Bind(&DevToolsWindow::ColorPickedInEyeDropper,
                                 base::Unretained(this))));
  } else {
    eye_dropper_.reset();
  }
}
