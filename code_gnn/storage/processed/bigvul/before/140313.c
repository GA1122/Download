void Editor::copyImage(const HitTestResult& result) {
  writeImageNodeToPasteboard(Pasteboard::generalPasteboard(),
                             result.innerNodeOrImageMapImage(),
                             result.altDisplayString());
}
