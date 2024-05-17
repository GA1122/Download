PrinterQuery::PrinterQuery()
    : ui_message_loop_(MessageLoop::current()),
      worker_(new PrintJobWorker(this)),
      is_print_dialog_box_shown_(false),
      cookie_(PrintSettings::NewCookie()),
      last_status_(PrintingContext::FAILED) {
}
