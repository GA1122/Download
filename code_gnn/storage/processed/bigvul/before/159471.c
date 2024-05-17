WebGLImageConversion::ImageExtractor::ImageExtractor(
    Image* image,
    ImageHtmlDomSource image_html_dom_source,
    bool premultiply_alpha,
    bool ignore_color_space) {
  image_ = image;
  image_html_dom_source_ = image_html_dom_source;
  ExtractImage(premultiply_alpha, ignore_color_space);
}
