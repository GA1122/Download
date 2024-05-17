  UkmRecorderFactoryImpl(ukm::mojom::UkmRecorderInterfacePtrInfo info)
      : info_(std::move(info)) {
    DCHECK(info_.is_valid());
  }
