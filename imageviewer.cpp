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
    updateStatus();
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
    viewToolBar = addToolBar("View");

    transformToolBar = new QToolBar();
    addToolBar(Qt::RightToolBarArea, transformToolBar);

    aFactorLabel = new QLabel(transformToolBar);
    aFactorLabel->setText("Factor 'a':");
    transformToolBar->addWidget(aFactorLabel);

    aFactorSlider = new QSlider(transformToolBar);
    aFactorSlider->setRange(-10, 10);
    aFactorSlider->setSliderPosition(1);
    aFactorSlider->setOrientation(Qt::Horizontal);
    transformToolBar->addWidget(aFactorSlider);

    bFactorLabel = new QLabel(transformToolBar);
    bFactorLabel->setText("Factor 'b':");
    transformToolBar->addWidget(bFactorLabel);

    bFactorSlider = new QSlider(transformToolBar);
    bFactorSlider->setRange(-50, 50);
    bFactorSlider->setSliderPosition(0);
    bFactorSlider->setOrientation(Qt::Horizontal);
    transformToolBar->addWidget(bFactorSlider);

    gammaFactorLabel = new QLabel(transformToolBar);
    gammaFactorLabel->setText("Factor 'gamma':");
    transformToolBar->addWidget(gammaFactorLabel);

    gammaFactorSlider = new QSlider(transformToolBar);
    gammaFactorSlider->setRange(-5, 5);
    gammaFactorSlider->setSliderPosition(1);
    gammaFactorSlider->setOrientation(Qt::Horizontal);
    transformToolBar->addWidget(gammaFactorSlider);

    connect(aFactorSlider, SIGNAL(valueChanged(int)), this, SLOT(updateImage()));
    connect(bFactorSlider, SIGNAL(valueChanged(int)), this, SLOT(updateImage()));
    connect(gammaFactorSlider, SIGNAL(valueChanged(int)), this, SLOT(updateImage()));

    // main area for image display
    imageScene = new QGraphicsScene(this);
    imageView = new QGraphicsView(imageScene);
    setCentralWidget(imageView);

    // setup status bar
    mainStatusBar = statusBar();
    mainStatusLabel = new QLabel(mainStatusBar);
    mainStatusBar->addPermanentWidget(mainStatusLabel);
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

void ImageViewer::updateStatus()
{
    QString status = QString("a=%1, b=%2, gamma=%3")
            .arg(int(getFactorA()))
            .arg(int(getFactorB()))
            .arg(int(getFactorGamma()));
    mainStatusLabel->setText(status);
}

void ImageViewer::zoomIn()
{
    imageView->scale(1.2, 1.2);
}

void ImageViewer::zoomOut()
{
    imageView->scale(1/1.2, 1/1.2);
}

void ImageViewer::prevImage()
{

}

void ImageViewer::nextImage()
{

}

void ImageViewer::updateImage()
{
    updateStatus();
}

void ImageViewer::saveAs()
{

}

float ImageViewer::getFactorA()
{
    if (aFactorSlider != nullptr)
    {
        return float(aFactorSlider->value());
    }
    return 1.0;
}

float ImageViewer::getFactorB()
{
    if (bFactorSlider != nullptr)
    {
        return float(bFactorSlider->value());
    }
    return 0;
}

float ImageViewer::getFactorGamma()
{
    if (gammaFactorSlider != nullptr)
    {
        return float(gammaFactorSlider->value());
    }
    return 1.0;
}
