   static void SetUpTestCase() {
     mb_ = reinterpret_cast<MACROBLOCKD*>(
        vpx_memalign(32, sizeof(MACROBLOCKD)));
    mi_ = reinterpret_cast<MODE_INFO*>(
        vpx_memalign(32, sizeof(MODE_INFO)));
    data_array_ = reinterpret_cast<uint8_t*>(
        vpx_memalign(kDataAlignment, kDataBufferSize));
 }
