void AppCacheUpdateJob::OnManifestInfoWriteComplete(int result) {
  if (result > 0) {
    scoped_refptr<net::StringIOBuffer> io_buffer(
        new net::StringIOBuffer(manifest_data_));
    manifest_response_writer_->WriteData(
        io_buffer.get(),
        manifest_data_.length(),
        base::Bind(&AppCacheUpdateJob::OnManifestDataWriteComplete,
                   base::Unretained(this)));
  } else {
    HandleCacheFailure(
        AppCacheErrorDetails("Failed to write the manifest headers to storage",
                     APPCACHE_UNKNOWN_ERROR,
                     GURL(),
                     0,
                     false  ),
        DISKCACHE_ERROR,
        GURL());
  }
}
