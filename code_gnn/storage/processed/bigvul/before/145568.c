  void RemoveCtapFlag() {
    scoped_feature_list_.emplace();
    scoped_feature_list_->InitAndDisableFeature(kNewCtap2Device);
  }
