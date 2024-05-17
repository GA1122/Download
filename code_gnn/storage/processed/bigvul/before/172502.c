CameraSource *CameraSource::Create(const String16 &clientName) {
 Size size;
    size.width = -1;
    size.height = -1;

    sp<ICamera> camera;
 return new CameraSource(camera, NULL, 0, clientName, -1,
            size, -1, NULL, false);
}
