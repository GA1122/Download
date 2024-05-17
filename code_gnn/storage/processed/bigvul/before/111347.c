void WebPage::setEnableLocalAccessToAllCookies(bool enabled)
{
    cookieManager().setCanLocalAccessAllCookies(enabled);
}
