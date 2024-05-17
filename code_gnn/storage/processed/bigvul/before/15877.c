static void msix_fire_vector_notifier(PCIDevice *dev,
                                      unsigned int vector, bool is_masked)
{
    MSIMessage msg;
    int ret;

    if (!dev->msix_vector_use_notifier) {
        return;
    }
    if (is_masked) {
        dev->msix_vector_release_notifier(dev, vector);
    } else {
        msg = msix_get_message(dev, vector);
        ret = dev->msix_vector_use_notifier(dev, vector, msg);
        assert(ret >= 0);
    }
}
