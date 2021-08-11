#include "imageviewer.h"
#include <QMenuBar>
#include <QApplication>
#include <QFileDialog>
#include <QImage>

ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    fileMenu(nullptr),
    currentImageContent(nullptr),
    currentImage(nullptr)
{
    initUI();
    initActions();
    initShortcuts();
}

ImageViewer::~ImageViewer()
{

}

void ImageViewer::initUI()
{
    this->resize(800, 600);
    // setup menubar
    fileMenu = menuBar()->addMenu("&File");
    viewMenu = menuBar()->addMenu("&Menu");

    // setup toolbar
    fileToolBar = addToolBar("File");
    viewToolBar = addToolBar("File");

    // main area for image display
    imageScene = new QGraphicsScene(this);
    imageView = new QGraphicsView(imageScene);
    setCentralWidget(imageView);

    // setup status bar
    mainStatusBar = statusBar();
    mainStatusLabel = new QLabel(mainStatusBar);
    mainStatusBar->addPermanentWidget(mainStatusLabel);
    mainStatusLabel->setText("Image Information will be here!");
}

void ImageViewer::initActions()
{
    // File Menu: open, saveAs, exit
    openAction = new QAction("&Open", this);
    fileMenu->addAction(openAction);
    saveAsAction = new QAction("&Save as", this);
    fileMenu->addAction(saveAsAction);
    exitAction = new QAction("E&xit", this);
    fileMenu->addAction(exitAction);

    zoomInAction = new QAction("Zoom in", this);
    viewMenu->addAction(zoomInAction);
    zoomOutAction = new QAction("Zoom Out", this);
    viewMenu->addAction(zoomOutAction);
    prevAction = new QAction("&Previous Image", this);
    viewMenu->addAction(prevAction);
    nextAction = new QAction("&Next Image", this);
    viewMenu->addAction(nextAction);

    // add actions to toolbars
    fileToolBar->addAction(openAction);
    viewToolBar->addAction(zoomInAction);
    viewToolBar->addAction(zoomOutAction);
    viewToolBar->addAction(prevAction);
    viewToolBar->addAction(nextAction);

    // connect the signals and slots
    connect(exitAction, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(openImage()));
    connect(saveAsAction, SIGNAL(triggered(bool)), this, SLOT(saveAs()));
    connect(zoomInAction, SIGNAL(triggered(bool)), this, SLOT(zoomIn()));
    connect(zoomOutAction, SIGNAL(triggered(bool)), this, SLOT(zoomOut()));
    connect(prevAction, SIGNAL(triggered(bool)), this, SLOT(prevImage()));
    connect(nextAction, SIGNAL(triggered(bool)), this, SLOT(nextImage()));
}

void ImageViewer::initShortcuts()
{
    QList<QKeySequence> shortcuts;
    shortcuts << Qt::Key_Plus << Qt::Key_Equal;
    zoomInAction->setShortcuts(shortcuts);

    shortcuts.clear();
    shortcuts << Qt::Key_Minus << Qt::Key_Underscore;
    zoomOutAction->setShortcuts(shortcuts);

    shortcuts.clear();
    shortcuts << Qt::Key_Up << Qt::Key_Left;
    prevAction->setShortcuts(shortcuts);

    shortcuts.clear();
    shortcuts << Qt::Key_Down << Qt::Key_Right;
    nextAction->setShortcuts(shortcuts);
}

void ImageViewer::openImage()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle("Open Image");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Images (*.png *.bmp *.jpg)"));
    QStringList filePaths;
    if (dialog.exec())
    {
        filePaths = dialog.selectedFiles();
        showImage(filePaths.at(0));
    }
}

void ImageViewer::showImage(QString path)
{
    // Clear
    imageScene->clear();
    imageView->resetTransform();

    // Set Image
    currentImageContent.load(path);
    QPixmap pixMap = QPixmap::fromImage(currentImageContent);
    currentImage = imageScene->addPixmap(pixMap);
    imageScene->update();
    imageView->setSceneRect(currentImageContent.rect());

    // Set status
    QString status = QString("%1, %2x%3, %4 Bytes")
            .arg(path).arg(currentImageContent.width())
            .arg(currentImageContent.height()).arg(QFile(path).size());
    mainStatusLabel->setText(status);
    currentImagePath = path;
}

void ImageViewer::zoomIn()
{
    imageView->scale(1.2, 1.2);
}

void ImageViewer::zoomOut()
{
    imageView->scale(1/1.2, 1/1.2);
}

void ImageViewer::saveAs()
{

}

