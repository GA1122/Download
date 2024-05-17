bool ExtensionApiTest::StartFTPServer(const base::FilePath& root_directory) {
  ftp_server_.reset(new net::SpawnedTestServer(net::SpawnedTestServer::TYPE_FTP,
                                               root_directory));

  if (!ftp_server_->Start())
    return false;

  test_config_->SetInteger(kFtpServerPort,
                           ftp_server_->host_port_pair().port());

  return true;
}
