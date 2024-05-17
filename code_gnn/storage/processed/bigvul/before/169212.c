void EnsureCookiesFlushed(BrowserContext* browser_context) {
  BrowserContext::ForEachStoragePartition(
      browser_context, base::BindRepeating([](StoragePartition* partition) {
        base::RunLoop run_loop;
        partition->GetCookieManagerForBrowserProcess()->FlushCookieStore(
            run_loop.QuitClosure());
        run_loop.Run();
      }));
}
