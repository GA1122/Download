ar6000_cookie_cleanup(struct ar6_softc *ar)
{
     
    ar->arCookieList = NULL;
    ar->arCookieCount = 0;
}
