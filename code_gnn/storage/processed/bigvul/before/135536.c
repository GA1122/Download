void Editor::CopyImage(const HitTestResult& result) {
  WriteImageNodeToPasteboard(Pasteboard::GeneralPasteboard(),
                             result.InnerNodeOrImageMapImage(),
                             result.AltDisplayString());
}
