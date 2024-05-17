SoftAVC::~SoftAVC() {
    CHECK_EQ(deInitDecoder(), (status_t)OK);
}
