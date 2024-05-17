GURL GetPromoResourceURL() {
  const std::string promo_server_url = CommandLine::ForCurrentProcess()->
      GetSwitchValueASCII(switches::kPromoServerURL);
  return promo_server_url.empty() ?
      NotificationPromo::PromoServerURL() : GURL(promo_server_url);
}
