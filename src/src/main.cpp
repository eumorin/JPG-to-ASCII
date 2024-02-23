#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QImage>
#include <qloggingcategory.h>

//void MsgHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
//    QByteArray localMsg = msg.toLocal8Bit();
//    switch (type) {
//    case QtDebugMsg:
//        fprintf(stderr, "Debug: %s\n", localMsg.constData());
//        break;
//    case QtWarningMsg:
//        fprintf(stderr, "Warning: %s\n", localMsg.constData());
//        break;
//    case QtCriticalMsg:
//        fprintf(stderr, "Critical: %s\n", localMsg.constData());
//        break;
//    case QtFatalMsg:
//        fprintf(stderr, "Fatal: %s\n", localMsg.constData());
//        abort();
//    }
//}

int main(int argc, char *argv[])
{
    /*QLoggingCategory::setFilterRules("*.debug=true\n"
        "driver.usb.debug=true");

    qputenv("QT_ASSUME_STDERR_HAS_CONSOLE", "1");*/

    /*qInstallMessageHandler(MsgHandler);*/

    if (argc < 2) {
        QApplication a(argc, argv);
        mainwindow w;
        w.show();

        return a.exec();
    }
    else {
        QApplication app(argc, argv);
        QApplication::setApplicationName("JPG-to-ASCII");

        QCommandLineParser parser;
        parser.setApplicationDescription("Generate ASCII art from the .png or .jpg image");
        parser.addHelpOption();

        QCommandLineOption inputFileOption(QStringList() << "f" << "file",
            "Path to input file. (string).",
            "file");
        parser.addOption(inputFileOption);

        QCommandLineOption widthOption(QStringList() << "W" << "width",
            "Width of the output. (integer).",
            "width");
        QCommandLineOption heightOption(QStringList() << "H" << "height",
            "Height of the output. (integer).",
            "height");
        parser.addOption(widthOption);
        parser.addOption(heightOption);

        QCommandLineOption outputOption(QStringList() << "o" << "output",
            "Path to save the result. (string).",
            "output");
        parser.addOption(outputOption);

        QCommandLineOption gradientOption(QStringList() << "g" << "gradient",
            "Character gradient for ASCII art. (string).",
            "gradient");
        gradientOption.setDefaultValue("@$8W9H4Z1l(r/!:.");
        parser.addOption(gradientOption);

        parser.process(app);

        bool inputProvided = parser.isSet(inputFileOption);
        bool outputProvided = parser.isSet(outputOption);

        bool widthOk, heightOk;
        int width = parser.value(widthOption).toInt(&widthOk);
        int height = parser.value(heightOption).toInt(&heightOk);

        if (!widthOk || !heightOk) {
            qWarning("Both -W (width) and -H (height) parameters must be integers");
            return 1;
        }

        bool widthProvided = parser.isSet(widthOption);
        bool heightProvided = parser.isSet(heightOption);

        if (!widthProvided || !heightProvided) {
            qWarning("Both -W (width) and -H (height) parameters must be provided");
            return 1;
        }

        QString inputFile = parser.value(inputFileOption);
        QFileInfo inputFileInfo(inputFile);
        QString baseName = inputFileInfo.baseName();

        QString outputPath = parser.value(outputOption);
        QFileInfo outputFileInfo(outputPath);
        if (outputFileInfo.isDir()) {
            outputPath += QDir::separator() + baseName + ".txt";
        }
        else if (outputFileInfo.suffix() != "txt") {
            outputPath += ".txt";
        }

        QImage image(inputFile);
        if (!image.isNull()) {
            int maxWidth = image.width();
            int maxHeight = image.height();

            if (width > maxWidth || height > maxHeight) {
                qWarning() << "Given height and width exceed the maximum allowable image size.";
                qWarning() << "Constraints: width =" << maxWidth << ", height =" << maxHeight;
                return 1;
            }
        }

        QString Qs = parser.value(gradientOption);
        if ((Qs.startsWith('"') && Qs.endsWith('"')) || (Qs.startsWith('\'') && Qs.endsWith('\''))) {
            Qs = Qs.mid(1, Qs.length() - 2);
        }
        string gradient = Qs.toStdString();
        mainwindow w;
        w.processCommandLineArguments(inputFile, width, height, gradient, outputPath);
        return 0;
    }
    
}
