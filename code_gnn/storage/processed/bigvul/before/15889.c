static void msix_update_function_masked(PCIDevice *dev)
{
    dev->msix_function_masked = !msix_enabled(dev) ||
        (dev->config[dev->msix_cap + MSIX_CONTROL_OFFSET] & MSIX_MASKALL_MASK);
}
