static Vector<WebPage*>* visibleWebPages()
{
    static Vector<WebPage*>* s_visibleWebPages = 0;  
    if (!s_visibleWebPages)
        s_visibleWebPages = new Vector<WebPage*>;
    return s_visibleWebPages;
}
