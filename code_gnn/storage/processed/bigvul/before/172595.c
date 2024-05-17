SoftHEVC::~SoftHEVC() {
    ALOGV("In SoftHEVC::~SoftHEVC");
    CHECK_EQ(deInitDecoder(), (status_t)OK);
}
