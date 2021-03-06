#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QMenu>
#include <QToolBar>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStatusBar>
#include <QAction>
#include <QString>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QImage>
#include <QSlider>

class ImageViewer : public QMainWindow
{
    Q_OBJECT

    /* Public methods */
public:
    ImageViewer(QWidget *parent=nullptr);
    ~ImageViewer();

    /* Private methods */
private:
    void initUI();
    void initActions();
    void initShortcuts();
    void showImage(QString);
    void updateStatus();
    float getFactorA();
    float getFactorB();
    float getFactorGamma();

    /* Private slots */
private slots:
    void openImage();
    void zoomIn();
    void zoomOut();
    void saveAs();
    void prevImage();
    void nextImage();
    void updateImage();

    /* Private fields */
private:
    QMenu *fileMenu;
    QMenu *viewMenu;

    QToolBar *fileToolBar;
    QToolBar *viewToolBar;
    QToolBar *transformToolBar;

    QGraphicsScene *imageScene;
    QGraphicsView *imageView;

    QStatusBar *mainStatusBar;
    QLabel *mainStatusLabel;

    QAction *openAction;
    QAction *saveAsAction;
    QAction *exitAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *prevAction;
    QAction *nextAction;

    QLabel *aFactorLabel;
    QSlider *aFactorSlider;

    QLabel *bFactorLabel;
    QSlider *bFactorSlider;

    QLabel *gammaFactorLabel;
    QSlider *gammaFactorSlider;

    QString currentImagePath;
    QImage *currentImageContent;
    QImage *updatedImageContent;
    QGraphicsPixmapItem *currentImage;
};

#endif // IMAGEVIEWER_H
