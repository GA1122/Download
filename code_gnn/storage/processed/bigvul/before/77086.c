parse_WRITE_ACTIONS(char *arg, struct ofpbuf *ofpacts,
                    enum ofputil_protocol *usable_protocols)
{
    size_t ofs = ofpacts_pull(ofpacts);
    struct ofpact_nest *on;
    char *error;

     
    ofpact_put(ofpacts, OFPACT_WRITE_ACTIONS, sizeof *on);
    ofpbuf_pull(ofpacts, sizeof *on);

     
    error = ofpacts_parse(arg, ofpacts, usable_protocols, false,
                          OFPACT_WRITE_ACTIONS);

     
    on = ofpbuf_push_uninit(ofpacts, sizeof *on);
    on->ofpact.len = ofpacts->size;

     
    ofpbuf_push_uninit(ofpacts, ofs);

    return error;
}
