bool IsFusionSensorType(mojom::SensorType type) {
  switch (type) {
    case mojom::SensorType::LINEAR_ACCELERATION:
    case mojom::SensorType::RELATIVE_ORIENTATION_EULER_ANGLES:
    case mojom::SensorType::RELATIVE_ORIENTATION_QUATERNION:
      return true;
    default:
      return false;
  }
}
