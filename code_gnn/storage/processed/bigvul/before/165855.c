ImageBitmapFactories& ImageBitmapFactories::FromInternal(GlobalObject& object) {
  ImageBitmapFactories* supplement =
      Supplement<GlobalObject>::template From<ImageBitmapFactories>(object);
  if (!supplement) {
    supplement = MakeGarbageCollected<ImageBitmapFactories>();
    Supplement<GlobalObject>::ProvideTo(object, supplement);
  }
  return *supplement;
}
