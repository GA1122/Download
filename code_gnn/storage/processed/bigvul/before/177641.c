 virtual void Predict(MB_PREDICTION_MODE mode) {
    mbptr_->mode_info_context->mbmi.uv_mode = mode;
    pred_fn_(mbptr_, data_ptr_[0] - kStride, data_ptr_[1] - kStride,
             data_ptr_[0] - 1, data_ptr_[1] - 1, kStride,

              data_ptr_[0], data_ptr_[1], kStride);
   }
