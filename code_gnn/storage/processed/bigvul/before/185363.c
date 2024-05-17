  float AudioParam::finalValue()
  {
    float value;
//     float value = m_value;
      calculateFinalValues(&value, 1, false);
      return value;
  }