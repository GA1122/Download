PrintingContext::Result PrintingContext::NewDocument(
    const std::wstring& document_name) {
  DCHECK(!in_print_job_);
  if (!context_)
    return OnError();

  abort_printing_ = false;

  in_print_job_ = true;

  if (SP_ERROR == SetAbortProc(context_, &AbortProc))
    return OnError();

  DOCINFO di = { sizeof(DOCINFO) };
  di.lpszDocName = document_name.c_str();

  std::wstring debug_dump_path = PrintedDocument::debug_dump_path();
  if (!debug_dump_path.empty()) {
    std::wstring filename;
    Time now(Time::Now());
    filename = base::TimeFormatShortDateNumeric(now);
    filename += L"_";
    filename += base::TimeFormatTimeOfDay(now);
    filename += L"_";
    filename += document_name;
    filename += L"_";
    filename += L"buffer.prn";
    file_util::ReplaceIllegalCharactersInPath(&filename, '_');
    file_util::AppendToPath(&debug_dump_path, filename);
    di.lpszOutput = debug_dump_path.c_str();
  }

  DCHECK(!MessageLoop::current() ? true :
      !MessageLoop::current()->NestableTasksAllowed());

  if (StartDoc(context_, &di) <= 0)
    return OnError();

#ifndef NDEBUG
  page_number_ = 0;
#endif
  return OK;
}
