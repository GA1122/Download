  device::ControllerFrameData GetCurrentControllerData(unsigned int index) {
    auto iter = controller_data_map_.find(index);
    DCHECK(iter != controller_data_map_.end());
    return iter->second;
  }
