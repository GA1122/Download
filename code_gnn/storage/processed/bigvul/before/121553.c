  DriveFileStreamReader::DriveFileSystemGetter GetDriveFileSystemGetter() {
    return base::Bind(&DriveFileStreamReaderTest::GetDriveFileSystem,
                      base::Unretained(this));
  }
