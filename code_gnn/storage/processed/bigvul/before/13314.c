pdf14_free_smask_color(pdf14_device * pdev)
{
    if (pdev->smaskcolor != NULL) {
        if ( pdev->smaskcolor->profiles != NULL) {
             
            gs_free_object(pdev->memory, pdev->smaskcolor->profiles,
                        "pdf14_free_smask_color");
        }
        gs_free_object(pdev->memory, pdev->smaskcolor, "pdf14_free_smask_color");
        pdev->smaskcolor = NULL;
    }
}
