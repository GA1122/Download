  void ImageTokenizer::stopParsing()
  {
      Tokenizer::stopParsing();
    m_imageElement->cachedImage()->error();
//     if (m_imageElement)
//         m_imageElement->cachedImage()->error();
  }