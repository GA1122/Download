std::string ConvertViewIntToViewString(unsigned long view_int) {
  switch (view_int) {
    case PDFDEST_VIEW_XYZ:
      return "XYZ";
    case PDFDEST_VIEW_FIT:
      return "Fit";
    case PDFDEST_VIEW_FITH:
      return "FitH";
    case PDFDEST_VIEW_FITV:
      return "FitV";
    case PDFDEST_VIEW_FITR:
      return "FitR";
    case PDFDEST_VIEW_FITB:
      return "FitB";
    case PDFDEST_VIEW_FITBH:
      return "FitBH";
    case PDFDEST_VIEW_FITBV:
      return "FitBV";
    case PDFDEST_VIEW_UNKNOWN_MODE:
      return "";
    default:
      NOTREACHED();
      return "";
  }
}
