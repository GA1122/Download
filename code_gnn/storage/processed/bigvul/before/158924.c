PDFiumEngine::FormFillTimerData::FormFillTimerData(base::TimeDelta period,
                                                   TimerCallback callback)
    : timer_period(period), timer_callback(callback) {}
