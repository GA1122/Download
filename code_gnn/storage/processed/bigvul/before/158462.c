  void SimulateMemoryPressure(
      base::MemoryPressureListener::MemoryPressureLevel level) {
    FrameEvictionManager::GetInstance()->OnMemoryPressure(level);
  }
