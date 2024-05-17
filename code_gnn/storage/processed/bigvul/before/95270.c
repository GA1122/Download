static int xfer_addmbox(struct findall_data *data, void *rock)
{
    if (!data) return 0;
    struct xfer_list *list = (struct xfer_list *) rock;

    if (!data->mbentry) {
         
        return 0;
    }

    if (list->part && strcmp(data->mbentry->partition, list->part)) {
         
        return 0;
    }

     
    if (!mbname_localpart(data->mbname) || list->allow_usersubs ||
        (!mbname_isdeleted(data->mbname) && !strarray_size(mbname_boxes(data->mbname)))) {
        const char *extname = mbname_extname(data->mbname, list->ns, list->userid);
        struct xfer_item *mbox = xzmalloc(sizeof(struct xfer_item));

        mbox->mbentry = mboxlist_entry_copy(data->mbentry);
        strncpy(mbox->extname, extname, sizeof(mbox->extname));
        if (mbname_localpart(data->mbname) && !list->allow_usersubs) {
             
            mbox->state = XFER_MOVING_USER;
        }

         
        mbox->next = list->mboxes;
        list->mboxes = mbox;
    }

    return 0;
}
