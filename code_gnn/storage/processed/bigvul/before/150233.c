  bool GetPostLayerVisibilityAndReset() {
    bool r = post_layer_visibility_;
    post_layer_visibility_ = false;
    return r;
  }
