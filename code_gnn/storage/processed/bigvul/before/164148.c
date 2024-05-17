  net::CompletionOnceCallback deletion_callback() {
    return base::BindOnce(&AppCacheServiceImplTest::OnDeleteAppCachesComplete,
                          base::Unretained(this));
  }
