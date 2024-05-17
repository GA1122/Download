CameraSource *CameraSource::CreateFromCamera(
 const sp<ICamera>& camera,
 const sp<ICameraRecordingProxy>& proxy,
 int32_t cameraId,
 const String16& clientName,
 uid_t clientUid,
 Size videoSize,
 int32_t frameRate,
 const sp<IGraphicBufferProducer>& surface,
 bool storeMetaDataInVideoBuffers) {

 CameraSource *source = new CameraSource(camera, proxy, cameraId,
            clientName, clientUid, videoSize, frameRate, surface,
            storeMetaDataInVideoBuffers);
 return source;
}
