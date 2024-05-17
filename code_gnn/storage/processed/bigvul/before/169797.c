exsltDateRegister (void)
{
    xsltRegisterExtModuleFunction ((const xmlChar *) "add",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateAddFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "add-duration",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateAddDurationFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "date",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDateFunction);
#ifdef WITH_TIME
    xsltRegisterExtModuleFunction ((const xmlChar *) "date-time",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDateTimeFunction);
#endif
    xsltRegisterExtModuleFunction ((const xmlChar *) "day-abbreviation",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDayAbbreviationFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "day-in-month",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDayInMonthFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "day-in-week",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDayInWeekFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "day-in-year",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDayInYearFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "day-name",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDayNameFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "day-of-week-in-month",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDayOfWeekInMonthFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "difference",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDifferenceFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "duration",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDurationFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "hour-in-day",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateHourInDayFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "leap-year",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateLeapYearFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "minute-in-hour",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateMinuteInHourFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "month-abbreviation",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateMonthAbbreviationFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "month-in-year",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateMonthInYearFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "month-name",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateMonthNameFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "second-in-minute",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateSecondInMinuteFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "seconds",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateSecondsFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "sum",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateSumFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "time",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateTimeFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "week-in-month",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateWeekInMonthFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "week-in-year",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateWeekInYearFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "year",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateYearFunction);
}
