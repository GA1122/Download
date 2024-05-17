void OutOfProcessInstance::GetPrintPresetOptionsFromDocument(
    PP_PdfPrintPresetOptions_Dev* options) {
  options->is_scaling_disabled = PP_FromBool(IsPrintScalingDisabled());
  options->duplex =
      static_cast<PP_PrivateDuplexMode_Dev>(engine_->GetDuplexType());
  options->copies = engine_->GetCopiesToPrint();
  pp::Size uniform_page_size;
  options->is_page_size_uniform =
      PP_FromBool(engine_->GetPageSizeAndUniformity(&uniform_page_size));
  options->uniform_page_size = uniform_page_size;
}
