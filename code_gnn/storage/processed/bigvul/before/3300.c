static int fd_sector_calc(uint8_t head, uint8_t track, uint8_t sect,
                          uint8_t last_sect, uint8_t num_sides)
{
    return (((track * num_sides) + head) * last_sect) + sect - 1;
}
