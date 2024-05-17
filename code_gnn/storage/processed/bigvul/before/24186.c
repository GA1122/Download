ar6000_free_cookie(struct ar6_softc *ar, struct ar_cookie * cookie)
{
     
    A_ASSERT(ar != NULL);
    A_ASSERT(cookie != NULL);

    cookie->arc_list_next = ar->arCookieList;
    ar->arCookieList = cookie;
    ar->arCookieCount++;
}