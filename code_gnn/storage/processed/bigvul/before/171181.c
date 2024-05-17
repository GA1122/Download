sp<IHDCP> MediaPlayerService::makeHDCP(bool createEncryptionModule) {
 return new HDCP(createEncryptionModule);
}
