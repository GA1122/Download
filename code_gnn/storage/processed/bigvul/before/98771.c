static void EncodeDragData(const WebDragData& data, bool add_data,
                           NPVariant* drag_type, NPVariant* drag_data) {
  const NPString* np_drag_type;
  if (data.hasFileNames()) {
    static const NPString kFiles = { "Files", 5 };
    np_drag_type = &kFiles;
  } else {
    static const NPString kEmpty = { "" , 0 };
    np_drag_type = &kEmpty;
    add_data = false;
  }

  STRINGN_TO_NPVARIANT(np_drag_type->UTF8Characters,
                       np_drag_type->UTF8Length,
                       *drag_type);
  if (!add_data) {
    VOID_TO_NPVARIANT(*drag_data);
    return;
  }

  WebVector<WebString> files;
  data.fileNames(files);

  static std::string utf8;
  utf8.clear();
  for (size_t i = 0; i < files.size(); ++i) {
    static const char kBackspaceDelimiter('\b');
    if (i != 0)
      utf8.append(1, kBackspaceDelimiter);
    utf8.append(files[i].utf8());
  }

  STRINGN_TO_NPVARIANT(utf8.data(), utf8.length(), *drag_data);
}
