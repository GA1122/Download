static bool is_good_opcode(uint8_t opcode)
{
    static uint8_t good_list[] = { 0x0, 0x1, 0x2, 0x8, 0x9, 0xa };
    for (size_t i = 0; i < sizeof(good_list); ++i) {
        if (opcode == good_list[i])
            return true;
    }
    return false;
}
