bool PDFiumEngine::HasPermission(DocumentPermission permission) const {
  if (permissions_handler_revision_ < 2)
    return true;

  if (permission == PERMISSION_PRINT_HIGH_QUALITY &&
      permissions_handler_revision_ >= 3) {
    return (permissions_ & kPDFPermissionPrintLowQualityMask) != 0 &&
           (permissions_ & kPDFPermissionPrintHighQualityMask) != 0;
  }

  switch (permission) {
    case PERMISSION_COPY:
      return (permissions_ & kPDFPermissionCopyMask) != 0;
    case PERMISSION_COPY_ACCESSIBLE:
      return (permissions_ & kPDFPermissionCopyAccessibleMask) != 0;
    case PERMISSION_PRINT_LOW_QUALITY:
    case PERMISSION_PRINT_HIGH_QUALITY:
      return (permissions_ & kPDFPermissionPrintLowQualityMask) != 0;
    default:
      return true;
  }
}
