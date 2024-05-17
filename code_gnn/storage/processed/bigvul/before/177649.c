 void SetupMacroblock(MACROBLOCKD *mbptr,
                       MODE_INFO *miptr,
 uint8_t *data,
 int block_size,
 int stride,
 int num_planes) {
    mbptr_ = mbptr;
    miptr_ = miptr;
    mbptr_->up_available = 1;
    mbptr_->left_available = 1;
    mbptr_->mode_info_context = miptr_;
    stride_ = stride;
    block_size_ = block_size;
    num_planes_ = num_planes;
 for (int p = 0; p < num_planes; p++)
      data_ptr_[p] = data + stride * (block_size + 1) * p +
                     stride + block_size;
 }
