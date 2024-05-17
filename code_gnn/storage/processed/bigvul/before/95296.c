AcpiNsValidateHandle (
    ACPI_HANDLE             Handle)
{

    ACPI_FUNCTION_ENTRY ();


     

    if ((!Handle) || (Handle == ACPI_ROOT_OBJECT))
    {
        return (AcpiGbl_RootNode);
    }

     

    if (ACPI_GET_DESCRIPTOR_TYPE (Handle) != ACPI_DESC_TYPE_NAMED)
    {
        return (NULL);
    }

    return (ACPI_CAST_PTR (ACPI_NAMESPACE_NODE, Handle));
}
