int main(int argc, char** argv)
{
    base::TestSuite testSuite(argc, argv);
    return base::LaunchUnitTests(argc, argv, base::Bind(runHelper, base::Unretained(&testSuite)));
}
