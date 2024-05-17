exsltDateXpathCtxtRegister (xmlXPathContextPtr ctxt, const xmlChar *prefix)
{
    if (ctxt
        && prefix
        && !xmlXPathRegisterNs(ctxt,
                               prefix,
                               (const xmlChar *) EXSLT_DATE_NAMESPACE)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "add",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateAddFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "add-duration",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateAddDurationFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "date",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDateFunction)
#ifdef WITH_TIME
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "date-time",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDateTimeFunction)
#endif
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "day-abbreviation",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDayAbbreviationFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "day-in-month",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDayInMonthFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "day-in-week",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDayInWeekFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "day-in-year",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDayInYearFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "day-name",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDayNameFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "day-of-week-in-month",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDayOfWeekInMonthFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "difference",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDifferenceFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "duration",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDurationFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "hour-in-day",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateHourInDayFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "leap-year",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateLeapYearFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "minute-in-hour",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateMinuteInHourFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "month-abbreviation",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateMonthAbbreviationFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "month-in-year",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateMonthInYearFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "month-name",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateMonthNameFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "second-in-minute",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateSecondInMinuteFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "seconds",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateSecondsFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "sum",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateSumFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "time",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateTimeFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "week-in-month",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateWeekInMonthFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "week-in-year",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateWeekInYearFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "year",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateYearFunction)) {
        return 0;
    }
    return -1;
}
