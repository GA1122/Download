  void PlatformSensorProviderAndroid::CreateAbsoluteOrientationQuaternionSensor(
      JNIEnv* env,
    mojo::ScopedSharedBufferMapping mapping,
//     SensorReadingSharedBuffer* reading_buffer,
      const CreateSensorCallback& callback) {
    ScopedJavaLocalRef<jobject> sensor = Java_PlatformSensorProvider_createSensor(
        env, j_object_,
        static_cast<jint>(mojom::SensorType::ABSOLUTE_ORIENTATION_QUATERNION));
  
    if (sensor.obj()) {
      auto concrete_sensor = base::MakeRefCounted<PlatformSensorAndroid>(
        mojom::SensorType::ABSOLUTE_ORIENTATION_QUATERNION, std::move(mapping),
//         mojom::SensorType::ABSOLUTE_ORIENTATION_QUATERNION, reading_buffer,
          this, sensor);
  
      callback.Run(concrete_sensor);
   } else {
     auto sensor_fusion_algorithm =
         std::make_unique<OrientationQuaternionFusionAlgorithmUsingEulerAngles>(
             true  );
  
    PlatformSensorFusion::Create(std::move(mapping), this,
//     PlatformSensorFusion::Create(reading_buffer, this,
                                   std::move(sensor_fusion_algorithm), callback);
    }
  }