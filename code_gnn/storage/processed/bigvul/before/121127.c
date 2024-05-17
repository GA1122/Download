String HTMLInputElement::capture() const
{
    if (!isFileUpload())
        return String();

    String capture = fastGetAttribute(captureAttr).lower();
    if (capture == "camera"
        || capture == "camcorder"
        || capture == "microphone"
        || capture == "filesystem")
        return capture;

    return "filesystem";
}
