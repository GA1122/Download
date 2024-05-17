ChromeContentBrowserClient::CreateRequestContextForStoragePartition(
    content::BrowserContext* browser_context,
    const base::FilePath& partition_path,
    bool in_memory,
    scoped_ptr<net::URLRequestJobFactory::ProtocolHandler>
        blob_protocol_handler,
    scoped_ptr<net::URLRequestJobFactory::ProtocolHandler>
        file_system_protocol_handler,
    scoped_ptr<net::URLRequestJobFactory::ProtocolHandler>
        developer_protocol_handler,
    scoped_ptr<net::URLRequestJobFactory::ProtocolHandler>
        chrome_protocol_handler,
    scoped_ptr<net::URLRequestJobFactory::ProtocolHandler>
        chrome_devtools_protocol_handler) {
  Profile* profile = Profile::FromBrowserContext(browser_context);
  return profile->CreateRequestContextForStoragePartition(
      partition_path,
      in_memory,
      blob_protocol_handler.Pass(),
      file_system_protocol_handler.Pass(),
      developer_protocol_handler.Pass(),
      chrome_protocol_handler.Pass(),
      chrome_devtools_protocol_handler.Pass());
}
