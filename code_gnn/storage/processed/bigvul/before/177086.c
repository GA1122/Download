SoftMPEG2::~SoftMPEG2() {
    CHECK_EQ(deInitDecoder(), (status_t)OK);
}
