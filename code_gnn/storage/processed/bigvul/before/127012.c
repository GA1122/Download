PassOwnPtr<ColorChooser> ChromeClientImpl::createColorChooser(ColorChooserClient* chooserClient, const Color&)
{
    OwnPtr<ColorChooserUIController> controller;
    if (RuntimeEnabledFeatures::pagePopupEnabled())
        controller = adoptPtr(new ColorChooserPopupUIController(this, chooserClient));
    else
        controller = adoptPtr(new ColorChooserUIController(this, chooserClient));
    controller->openUI();
    return controller.release();
}
