static uint8_t msix_pending_mask(int vector)
{
    return 1 << (vector % 8);
}
