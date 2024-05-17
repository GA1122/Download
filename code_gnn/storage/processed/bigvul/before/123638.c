DeviceOrientationController* DeviceOrientationController::from(Document* document)
{
    DeviceOrientationController* controller = static_cast<DeviceOrientationController*>(DocumentSupplement::from(document, supplementName()));
    if (!controller) {
        controller = new DeviceOrientationController(document);
        DocumentSupplement::provideTo(document, supplementName(), adoptPtr(controller));
    }
    return controller;
}
