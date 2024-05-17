poppler_page_get_text_output_dev (PopplerPage *page)
{
  if (page->text_dev == NULL) {
    page->text_dev = new TextOutputDev (NULL, gTrue, gFalse, gFalse);

    if (page->gfx)
      delete page->gfx;
    page->gfx = page->page->createGfx(page->text_dev,
				      72.0, 72.0, 0,
				      gFalse,  
				      gTrue,  
				      -1, -1, -1, -1,
				      gFalse,  
				      page->document->doc->getCatalog (),
				      NULL, NULL, NULL, NULL);

    page->page->display(page->gfx);

    page->text_dev->endPage();
  }

  return page->text_dev;
}
