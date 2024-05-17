pdf14_output_page(gx_device * dev, int num_copies, int flush)
{
    pdf14_device * pdev = (pdf14_device *)dev;

    if (pdev->target != NULL)
        return (*dev_proc(pdev->target, output_page)) (pdev->target, num_copies, flush);
    return 0;
}
