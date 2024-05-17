bool IsLinkArea(PDFiumPage::Area area) {
  return area == PDFiumPage::WEBLINK_AREA || area == PDFiumPage::DOCLINK_AREA;
}
