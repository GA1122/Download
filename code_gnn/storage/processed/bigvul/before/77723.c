parse_instruction_ids(struct ofpbuf *payload, bool loose, uint32_t *insts)
{
    *insts = 0;
    while (payload->size > 0) {
        enum ovs_instruction_type inst;
        enum ofperr error;
        uint64_t ofpit;

         
        error = ofpprop_pull__(payload, NULL, 1, 0x10000, &ofpit);
        if (error) {
            return error;
        }

        error = ovs_instruction_type_from_inst_type(&inst, ofpit);
        if (!error) {
            *insts |= 1u << inst;
        } else if (!loose) {
            return error;
        }
    }
    return 0;
}
