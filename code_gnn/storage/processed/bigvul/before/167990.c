void LocalFrame::StartPrintingWithoutPrintingLayout() {
  SetPrinting( true,  false, FloatSize(),
              FloatSize(), 0);
}
