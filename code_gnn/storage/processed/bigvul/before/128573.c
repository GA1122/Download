std::vector<pp::ImageData> Instance::GetProgressBarResources(
    pp::ImageData* background) {
  std::vector<pp::ImageData> result(9);
  result[0] = CreateResourceImage(PP_RESOURCEIMAGE_PDF_PROGRESS_BAR_0);
  result[1] = CreateResourceImage(PP_RESOURCEIMAGE_PDF_PROGRESS_BAR_1);
  result[2] = CreateResourceImage(PP_RESOURCEIMAGE_PDF_PROGRESS_BAR_2);
  result[3] = CreateResourceImage(PP_RESOURCEIMAGE_PDF_PROGRESS_BAR_3);
  result[4] = CreateResourceImage(PP_RESOURCEIMAGE_PDF_PROGRESS_BAR_4);
  result[5] = CreateResourceImage(PP_RESOURCEIMAGE_PDF_PROGRESS_BAR_5);
  result[6] = CreateResourceImage(PP_RESOURCEIMAGE_PDF_PROGRESS_BAR_6);
  result[7] = CreateResourceImage(PP_RESOURCEIMAGE_PDF_PROGRESS_BAR_7);
  result[8] = CreateResourceImage(PP_RESOURCEIMAGE_PDF_PROGRESS_BAR_8);
  *background = CreateResourceImage(
      PP_RESOURCEIMAGE_PDF_PROGRESS_BAR_BACKGROUND);
  return result;
}
