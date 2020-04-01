#include <QCommandLineParser>
#include <QGuiApplication>
#include <QQuickItem>
#include <QQuickItemGrabResult>
#include <QQuickView>
#include <QUrl>
#include <QWindow>
#include <QtDebug>
#include <set>

namespace {
void grabItemsRecursively(std::set<QSharedPointer<QQuickItemGrabResult>> &grabResults,
                          QQuickItem *item)
{
    for (auto *child : item->childItems()) {
        grabItemsRecursively(grabResults, child);
    }

    const auto fileName = item->property("fileName").toString();
    if (fileName.isEmpty())
        return;
    auto grab = item->grabToImage();
    if (!grab) {
        qWarning() << "Failed to grab item" << item;
        return;
    }

    grabResults.insert(grab);
    QObject::connect(grab.get(), &QQuickItemGrabResult::ready, [fileName, grab, &grabResults]() {
        auto ok = grab->saveToFile(fileName);
        if (ok) {
            qInfo() << "Saved file" << fileName;
        } else {
            qWarning() << "Failed to save file" << fileName;
        }
        grabResults.erase(grab);
        if (grabResults.empty()) {
            QCoreApplication::quit();
        }
    });
}
} // namespace

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument("filename", "Source QML file to process.");
    parser.process(app);

    const auto arguments = parser.positionalArguments();
    if (arguments.empty()) {
        qWarning() << "No filename specified";
        return -1;
    }

    const auto fileName = arguments.front();

    QWindow dummyWindow;
    dummyWindow.create();
    QQuickView offscreenView(QUrl::fromLocalFile(fileName), &dummyWindow);
    auto *rootItem = offscreenView.rootObject();
    if (!rootItem) {
        qWarning() << "No root item created from" << fileName;
        return -1;
    }
    // the parent window has to be visible to grabToImage()
    offscreenView.show();

    std::set<QSharedPointer<QQuickItemGrabResult>> grabResults;
    grabItemsRecursively(grabResults, rootItem);
    if (grabResults.empty()) {
        qWarning() << "No item to grab found in " << fileName;
        qWarning() << "(Set fileName property to items to be grabbed.)";
        return -1;
    }
    qInfo() << "Found" << grabResults.size() << "items to grab";

    return app.exec();
}
