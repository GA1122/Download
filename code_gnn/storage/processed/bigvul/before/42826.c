static void init_pages(void)
{
    init_page(&pages[0], PAGE_SUMMARY            , _("Problem description")   );
    init_page(&pages[1], PAGE_EVENT_SELECTOR     , _("Select how to report this problem"));
    init_page(&pages[2], PAGE_EDIT_COMMENT,_("Provide additional information"));
    init_page(&pages[3], PAGE_EDIT_ELEMENTS      , _("Review the data")       );
    init_page(&pages[4], PAGE_REVIEW_DATA        , _("Confirm data to report"));
    init_page(&pages[5], PAGE_EVENT_PROGRESS     , _("Processing")            );
    init_page(&pages[6], PAGE_EVENT_DONE         , _("Processing done")       );
    init_page(&pages[7], PAGE_NOT_SHOWN          , ""                         );
}