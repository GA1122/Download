static void msix_handle_mask_update(PCIDevice *dev, int vector, bool was_masked)
{
    bool is_masked = msix_is_masked(dev, vector);

    if (is_masked == was_masked) {
        return;
    }

    msix_fire_vector_notifier(dev, vector, is_masked);

    if (!is_masked && msix_is_pending(dev, vector)) {
        msix_clr_pending(dev, vector);
        msix_notify(dev, vector);
    }
}
