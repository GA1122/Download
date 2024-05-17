  void FetchSources() {
    for (mojom::SensorType type : fusion_algorithm_->source_types()) {
      scoped_refptr<PlatformSensor> sensor = provider_->GetSensor(type);
      if (sensor) {
        SensorCreated(std::move(sensor));
      } else {
        provider_->CreateSensor(type,
                                base::Bind(&Factory::SensorCreated, this));
      }
    }
  }
