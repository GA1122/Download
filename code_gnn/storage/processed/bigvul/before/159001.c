FPDF_BOOL PDFiumEngine::Pause_NeedToPauseNow(IFSDK_PAUSE* param) {
  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  return base::Time::Now() - engine->last_progressive_start_time_ >
         engine->progressive_paint_timeout_;
}
