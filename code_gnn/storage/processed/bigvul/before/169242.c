std::string GetCookies(BrowserContext* browser_context, const GURL& url) {
  std::string cookies;
  base::RunLoop run_loop;
  network::mojom::CookieManagerPtr cookie_manager;
  BrowserContext::GetDefaultStoragePartition(browser_context)
      ->GetNetworkContext()
      ->GetCookieManager(mojo::MakeRequest(&cookie_manager));
  cookie_manager->GetCookieList(
      url, net::CookieOptions(),
      base::BindOnce(&GetCookiesCallback, &cookies, &run_loop));
  run_loop.Run();
  return cookies;
}
