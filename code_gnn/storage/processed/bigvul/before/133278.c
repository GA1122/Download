RenderSVGImage::~RenderSVGImage()
{
    ImageQualityController::remove(this);
    m_imageResource->shutdown();
}
