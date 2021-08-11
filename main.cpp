#include <QApplication>
#include <QString>
#include "imageviewer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ImageViewer viewer;
    viewer.setWindowTitle("Image Viewer");
    viewer.show();
    return app.exec();
}
