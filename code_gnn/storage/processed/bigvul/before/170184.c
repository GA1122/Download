void ComponentUpdaterPolicyTest::CallAsync(TestCaseAction action) {
  base::PostTaskWithTraits(FROM_HERE, {BrowserThread::UI},
                           base::BindOnce(action, base::Unretained(this)));
}
