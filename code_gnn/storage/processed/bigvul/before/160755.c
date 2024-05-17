MhtmlSaveStatus WriteMHTMLToDisk(std::vector<WebThreadSafeData> mhtml_contents,
                                 base::File file) {
  TRACE_EVENT0("page-serialization", "WriteMHTMLToDisk (RenderFrameImpl)");
  SCOPED_UMA_HISTOGRAM_TIMER(
      "PageSerialization.MhtmlGeneration.WriteToDiskTime.SingleFrame");
  DCHECK(!RenderThread::Get()) << "Should not run in the main renderer thread";
  MhtmlSaveStatus save_status = MhtmlSaveStatus::SUCCESS;
  for (const WebThreadSafeData& data : mhtml_contents) {
    if (!data.IsEmpty() &&
        file.WriteAtCurrentPos(data.Data(), data.size()) < 0) {
      save_status = MhtmlSaveStatus::FILE_WRITTING_ERROR;
      break;
    }
  }
  file.Close();
  return save_status;
}
