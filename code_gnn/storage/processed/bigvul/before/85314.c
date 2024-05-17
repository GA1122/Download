apply_printer_defaults(
    cupsd_printer_t *printer,		 
    cupsd_job_t     *job)		 
{
  int		i,			 
		num_options;		 
  cups_option_t	*options,		 
		*option;		 


  cupsdLogJob(job, CUPSD_LOG_DEBUG, "Applying default options...");

  

  for (i = printer->num_options, num_options = 0, options = NULL,
           option = printer->options;
       i > 0;
       i --, option ++)
    if (!ippFindAttribute(job->attrs, option->name, IPP_TAG_ZERO))
    {
      if (!strcmp(option->name, "print-quality") && ippFindAttribute(job->attrs, "cupsPrintQuality", IPP_TAG_NAME))
        continue;                      

      cupsdLogJob(job, CUPSD_LOG_DEBUG, "Adding default %s=%s", option->name, option->value);

      num_options = cupsAddOption(option->name, option->value, num_options, &options);
    }

  

  cupsEncodeOptions2(job->attrs, num_options, options, IPP_TAG_JOB);
  cupsFreeOptions(num_options, options);
}
