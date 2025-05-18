#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "smartpointer.h"
#include <QMainWindow>
#include <QImage>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool loadImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionExit_triggered();
    void on_actionPencil_triggered();
    void on_actionLine_triggered();
    void on_actionRectangle_triggered();
    void on_actionEllipse_triggered();
    void on_actionColor_triggered();
    void on_actionPenWidth_triggered();

private:
    SmartPointer<Ui::MainWindow> ui; // Наш умный указатель вместо сырого
    void drawTo(const QPoint &endPoint);
    void drawShape(const QPoint &endPoint);
    bool maybeSave();
    void clearImage();

    QImage image;
    QPoint lastPoint;
    QColor penColor;
    int penWidth;
    bool drawing;
    QString currentFile;

    enum Tool { Pencil, Line, Rectangle, Ellipse };
    Tool currentTool;
};

#endif // MAINWINDOW_H
