PassOwnPtr<GIFImageDecoder> createDecoder()
{
    return adoptPtr(new GIFImageDecoder(ImageSource::AlphaNotPremultiplied, ImageSource::GammaAndColorProfileApplied, ImageDecoder::noDecodedImageByteLimit));
}
