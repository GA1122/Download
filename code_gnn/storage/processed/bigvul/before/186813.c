  void PlatformSensorProviderAndroid::CreateRelativeOrientationEulerAnglesSensor(
      JNIEnv* env,
    mojo::ScopedSharedBufferMapping mapping,
//     SensorReadingSharedBuffer* reading_buffer,
      const CreateSensorCallback& callback) {
    if (static_cast<bool>(Java_PlatformSensorProvider_hasSensorType(
            env, j_object_,
           static_cast<jint>(
               mojom::SensorType::RELATIVE_ORIENTATION_QUATERNION)))) {
     auto sensor_fusion_algorithm =
         std::make_unique<OrientationEulerAnglesFusionAlgorithmUsingQuaternion>(
             false  );
  
    PlatformSensorFusion::Create(std::move(mapping), this,
//     PlatformSensorFusion::Create(reading_buffer, this,
                                   std::move(sensor_fusion_algorithm), callback);
    } else {
      callback.Run(nullptr);
   }
 }