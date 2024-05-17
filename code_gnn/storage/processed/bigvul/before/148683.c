void SkiaOutputSurfaceImplTest::CopyRequestCallbackOnGpuThread(
    const SkColor output_color,
    const gfx::Rect& output_rect,
    const gfx::ColorSpace& color_space,
    std::unique_ptr<CopyOutputResult> result) {
  std::unique_ptr<SkBitmap> result_bitmap;
  result_bitmap = std::make_unique<SkBitmap>(result->AsSkBitmap());
  EXPECT_EQ(result_bitmap->width(), output_rect.width());
  EXPECT_EQ(result_bitmap->height(), output_rect.height());

  std::vector<SkPMColor> expected_pixels(
      output_rect.width() * output_rect.height(),
      SkPreMultiplyColor(output_color));
  SkBitmap expected;
  expected.installPixels(
      SkImageInfo::MakeN32Premul(output_rect.width(), output_rect.height(),
                                 color_space.ToSkColorSpace()),
      expected_pixels.data(), output_rect.width() * sizeof(SkColor));
  ExpectEquals(*result_bitmap.get(), expected);

  UnblockMainThread();
}
