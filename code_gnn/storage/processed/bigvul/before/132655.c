void BlinkTestRunner::OnTryLeakDetection() {
  WebLocalFrame* main_frame =
      render_view()->GetWebView()->mainFrame()->toWebLocalFrame();
  DCHECK_EQ(GURL(url::kAboutBlankURL), GURL(main_frame->document().url()));
  DCHECK(!main_frame->isLoading());

  leak_detector_->TryLeakDetection(main_frame);
}
