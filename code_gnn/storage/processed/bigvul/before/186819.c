  void PlatformSensorProviderLinux::CreateFusionSensor(
      mojom::SensorType type,
    mojo::ScopedSharedBufferMapping mapping,
//     SensorReadingSharedBuffer* reading_buffer,
      const CreateSensorCallback& callback) {
    DCHECK(IsFusionSensorType(type));
    std::unique_ptr<PlatformSensorFusionAlgorithm> fusion_algorithm;
   switch (type) {
     case mojom::SensorType::LINEAR_ACCELERATION:
       fusion_algorithm = std::make_unique<
           LinearAccelerationFusionAlgorithmUsingAccelerometer>();
       break;
     case mojom::SensorType::RELATIVE_ORIENTATION_EULER_ANGLES:
       fusion_algorithm = std::make_unique<
           RelativeOrientationEulerAnglesFusionAlgorithmUsingAccelerometer>();
       break;
     case mojom::SensorType::RELATIVE_ORIENTATION_QUATERNION:
       fusion_algorithm = std::make_unique<
           OrientationQuaternionFusionAlgorithmUsingEulerAngles>(
           false  );
       break;
     default:
       NOTREACHED();
    }
  
    DCHECK(fusion_algorithm);
  PlatformSensorFusion::Create(std::move(mapping), this,
//   PlatformSensorFusion::Create(reading_buffer, this,
                                 std::move(fusion_algorithm), callback);
  }