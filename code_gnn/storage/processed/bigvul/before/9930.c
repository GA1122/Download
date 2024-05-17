void Part::slotExtractionDone(KJob* job)
{
    if (job->error() && job->error() != KJob::KilledJobError) {
        KMessageBox::error(widget(), job->errorString());
    } else {
        ExtractJob *extractJob = qobject_cast<ExtractJob*>(job);
        Q_ASSERT(extractJob);

        if (ArkSettings::openDestinationFolderAfterExtraction()) {
            qCDebug(ARK) << "Shall open" << extractJob->destinationDirectory();
            QUrl destinationDirectory = QUrl::fromLocalFile(extractJob->destinationDirectory()).adjusted(QUrl::NormalizePathSegments);
            qCDebug(ARK) << "Shall open URL" << destinationDirectory;

            KRun::runUrl(destinationDirectory, QStringLiteral("inode/directory"), widget());
        }

        if (ArkSettings::closeAfterExtraction()) {
           emit quit();
        }
    }
}
