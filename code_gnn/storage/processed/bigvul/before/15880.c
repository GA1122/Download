bool msix_is_masked(PCIDevice *dev, unsigned int vector)
{
    return msix_vector_masked(dev, vector, dev->msix_function_masked);
}
