  void SetCertToSHA1() {
    cert_ =
        net::ImportCertFromFile(net::GetTestCertsDirectory(), "sha1_leaf.pem");
    ASSERT_TRUE(cert_);
  }
