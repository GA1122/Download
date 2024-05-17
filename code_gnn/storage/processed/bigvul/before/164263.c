void AppCacheUpdateJob::OnManifestInfoWriteComplete(int result) {
  if (result > 0) {
    scoped_refptr<net::StringIOBuffer> io_buffer =
        base::MakeRefCounted<net::StringIOBuffer>(manifest_data_);
    manifest_response_writer_->WriteData(
        io_buffer.get(), manifest_data_.length(),
        base::BindOnce(&AppCacheUpdateJob::OnManifestDataWriteComplete,
                       base::Unretained(this)));
  } else {
    HandleCacheFailure(
        blink::mojom::AppCacheErrorDetails(
            "Failed to write the manifest headers to storage",
            blink::mojom::AppCacheErrorReason::APPCACHE_UNKNOWN_ERROR, GURL(),
            0, false  ),
        DISKCACHE_ERROR, GURL());
  }
}
