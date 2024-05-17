TestRunner::TestRunner(JobLevel job_level, TokenLevel startup_token,
                       TokenLevel main_token) : is_init_(false) {
  Init(job_level, startup_token, main_token);
}
