bool WebPage::isEnableLocalAccessToAllCookies() const
{
    return cookieManager().canLocalAccessAllCookies();
}
