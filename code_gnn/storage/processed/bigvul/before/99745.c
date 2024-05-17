void AddV4L2GpuWhitelist(std::vector<BrokerFilePermission>* permissions) {
  if (IsAcceleratedVideoDecodeEnabled()) {
    static const char kDevVideoDecPath[] = "/dev/video-dec";
    permissions->push_back(BrokerFilePermission::ReadWrite(kDevVideoDecPath));
  }

  static const char kDevVideoEncPath[] = "/dev/video-enc";
  permissions->push_back(BrokerFilePermission::ReadWrite(kDevVideoEncPath));

  static const char kDevJpegDecPath[] = "/dev/jpeg-dec";
  permissions->push_back(BrokerFilePermission::ReadWrite(kDevJpegDecPath));
}
