void TranslateInfoBarBase::FadeBackground(gfx::Canvas* canvas,
                                          double animation_value,
                                          const InfoBarBackground& background) {
  canvas->SaveLayerAlpha(static_cast<int>(animation_value * 255));
  canvas->AsCanvasSkia()->drawARGB(0, 255, 255, 255, SkXfermode::kClear_Mode);
  background.Paint(canvas, this);
  canvas->Restore();
}
