ImageBitmapFactories& ImageBitmapFactories::fromInternal(DOMWindow& object)
{
    ImageBitmapFactories* supplement = static_cast<ImageBitmapFactories*>(Supplement<DOMWindow>::from(object, supplementName()));
    if (!supplement) {
        supplement = new ImageBitmapFactories();
        Supplement<DOMWindow>::provideTo(object, supplementName(), adoptPtr(supplement));
    }
    return *supplement;
}
