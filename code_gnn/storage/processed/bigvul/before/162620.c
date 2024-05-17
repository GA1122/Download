void HeadlessDevToolsManagerDelegate::BeginFrame(
    content::DevToolsAgentHost* agent_host,
    int session_id,
    int command_id,
    const base::DictionaryValue* params,
    const CommandCallback& callback) {
  DCHECK(callback);

  content::WebContents* web_contents = agent_host->GetWebContents();
  if (!web_contents) {
    callback.Run(CreateErrorResponse(command_id, kErrorServerError,
                                     "Command not supported on this endpoint"));
    return;
  }

  HeadlessWebContentsImpl* headless_contents =
      HeadlessWebContentsImpl::From(browser_.get(), web_contents);
  if (!headless_contents->begin_frame_control_enabled()) {
    callback.Run(CreateErrorResponse(
        command_id, kErrorServerError,
        "Command is only supported if BeginFrameControl is enabled."));
    return;
  }

  base::Time frame_time;
  base::TimeTicks frame_timeticks;
  base::TimeTicks deadline;
  base::TimeDelta interval;

  if (const base::Value* frame_time_value = params->FindKey("frameTime")) {
    frame_time = base::Time::FromJsTime(frame_time_value->GetDouble());
    base::TimeDelta delta = frame_time - base::Time::UnixEpoch();
    frame_timeticks = base::TimeTicks::UnixEpoch() + delta;
  } else {
    frame_timeticks = base::TimeTicks::Now();
  }

  if (const base::Value* interval_value = params->FindKey("interval")) {
    double interval_double = interval_value->GetDouble();
    if (interval_double <= 0) {
      callback.Run(CreateErrorResponse(command_id, kErrorInvalidParam,
                                       "interval has to be greater than 0"));
      return;
    }
    interval = base::TimeDelta::FromMillisecondsD(interval_double);
  } else {
    interval = viz::BeginFrameArgs::DefaultInterval();
  }

  if (const base::Value* deadline_value = params->FindKey("deadline")) {
    base::TimeDelta delta =
        base::Time::FromDoubleT(deadline_value->GetDouble()) - frame_time;
    if (delta <= base::TimeDelta()) {
      callback.Run(CreateErrorResponse(command_id, kErrorInvalidParam,
                                       "deadline has to be after frameTime"));
      return;
    }
    deadline = frame_timeticks + delta;
  } else {
    deadline = frame_timeticks + interval;
  }

  bool capture_screenshot = false;
  ImageEncoding encoding = ImageEncoding::kPng;
  int quality = kDefaultScreenshotQuality;

  const base::Value* value = nullptr;
  const base::DictionaryValue* screenshot_dict = nullptr;
  if (params->Get("screenshot", &value)) {
    if (!value->GetAsDictionary(&screenshot_dict)) {
      callback.Run(CreateInvalidParamResponse(command_id, "screenshot"));
      return;
    }

    capture_screenshot = true;

    if (const base::Value* format_value = screenshot_dict->FindKey("format")) {
      const std::string& format = format_value->GetString();
      if (format == kPng) {
        encoding = ImageEncoding::kPng;
      } else if (format == kJpeg) {
        encoding = ImageEncoding::kJpeg;
      } else {
        callback.Run(
            CreateInvalidParamResponse(command_id, "screenshot.format"));
        return;
      }
    }

    if (const base::Value* quality_value = screenshot_dict->FindKey("quality")) {
      quality = quality_value->GetInt();
      if (quality < 0 || quality > 100) {
        callback.Run(CreateErrorResponse(
            command_id, kErrorInvalidParam,
            "screenshot.quality has to be in range 0..100"));
        return;
      }
    }
  }

  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          cc::switches::kRunAllCompositorStagesBeforeDraw) &&
      headless_contents->HasPendingFrame()) {
    LOG(WARNING) << "A BeginFrame is already in flight. In "
                    "--run-all-compositor-stages-before-draw mode, only a "
                    "single BeginFrame should be active at the same time.";
  }

  headless_contents->BeginFrame(frame_timeticks, deadline, interval,
                                capture_screenshot,
                                base::Bind(&OnBeginFrameFinished, command_id,
                                           callback, encoding, quality));
}
