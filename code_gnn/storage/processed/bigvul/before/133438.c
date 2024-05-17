SVGImage::~SVGImage()
{
    if (m_page) {
        OwnPtr<Page> currentPage = m_page.release();
        currentPage->mainFrame()->loader().frameDetached();  
    }

    ASSERT(!m_chromeClient || !m_chromeClient->image());
}
