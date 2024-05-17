void AwContents::CreatePdfExporter(JNIEnv* env,
                                   jobject obj,
                                   jobject pdfExporter) {
  pdf_exporter_.reset(
      new AwPdfExporter(env,
                        pdfExporter,
                        web_contents_.get()));
}
