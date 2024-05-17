bool SetCookie(BrowserContext* browser_context,
               const GURL& url,
               const std::string& value) {
  bool result = false;
  base::RunLoop run_loop;
  network::mojom::CookieManagerPtr cookie_manager;
  BrowserContext::GetDefaultStoragePartition(browser_context)
      ->GetNetworkContext()
      ->GetCookieManager(mojo::MakeRequest(&cookie_manager));
  std::unique_ptr<net::CanonicalCookie> cc(net::CanonicalCookie::Create(
      url, value, base::Time::Now(), net::CookieOptions()));
  DCHECK(cc.get());

  cookie_manager->SetCanonicalCookie(
      *cc.get(), true  , true  ,
      base::BindOnce(&SetCookieCallback, &result, &run_loop));
  run_loop.Run();
  return result;
}
