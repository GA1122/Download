WebString BlinkTestRunner::RegisterIsolatedFileSystem(
    const blink::WebVector<blink::WebString>& absolute_filenames) {
  std::vector<base::FilePath> files;
  for (size_t i = 0; i < absolute_filenames.size(); ++i)
    files.push_back(base::FilePath::FromUTF16Unsafe(absolute_filenames[i]));
  std::string filesystem_id;
  Send(new LayoutTestHostMsg_RegisterIsolatedFileSystem(
      routing_id(), files, &filesystem_id));
  return WebString::fromUTF8(filesystem_id);
}
