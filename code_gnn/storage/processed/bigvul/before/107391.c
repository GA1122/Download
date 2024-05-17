int Scrollbar::maxOverlapBetweenPages()
{
    static int maxOverlapBetweenPages = ScrollbarTheme::nativeTheme()->maxOverlapBetweenPages();
    return maxOverlapBetweenPages;
}
