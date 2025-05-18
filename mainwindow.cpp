#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileInfo>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), // SmartPtr ведет себя как обычный указатель
    penColor(Qt::black),
    penWidth(2),
    drawing(false),
    currentTool(Pencil)
{
    ui->setupUi(this);
    image = QImage(this->size(), QImage::Format_RGB32);
    clearImage();
}

MainWindow::~MainWindow() {}

bool MainWindow::loadImage(const QString &fileName) {
    QImage loadedImage;

    if (!loadedImage.load(fileName)) {
        QMessageBox::warning(this, tr("Error"), tr("Could not load %1").arg(fileName));
        return false;
    }

    image = loadedImage.convertToFormat(QImage::Format_RGB32);
    currentFile = fileName;
    update();
    return true;
}

bool MainWindow::saveImage(const QString &fileName, const char *fileFormat) {
    if (!image.save(fileName, fileFormat)) {
        QMessageBox::warning(this, tr("Error"), tr("Could not save %1").arg(fileName));
        return false;
    }
    currentFile = fileName;
    return true;
}

void MainWindow::clearImage() {
    image.fill(Qt::white);
    update();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(0, 0, image);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        drawing = true;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) && drawing) {
        if (currentTool == Pencil) {
            drawTo(event->pos());
        } else {
            update();
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && drawing) {
        if (currentTool != Pencil) {
            drawShape(event->pos());
        }
        drawing = false;
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

bool MainWindow::maybeSave() {
    if (image == QImage(this->size(), QImage::Format_RGB32) && image.allGray()) {
        return true;
    }

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Application"),
                               tr("The image has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save) {
        on_actionSave_triggered();
        return true;
    } else if (ret == QMessageBox::Cancel) {
        return false;
    }
    return true;
}

void MainWindow::drawTo(const QPoint &endPoint) {
    QPainter painter(&image);
    painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    lastPoint = endPoint;
    update();
}

void MainWindow::drawShape(const QPoint &endPoint) {
    QPainter painter(&image);
    painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    switch (currentTool) {
    case Line:
        painter.drawLine(lastPoint, endPoint);
        break;
    case Rectangle:
        painter.drawRect(QRect(lastPoint, endPoint));
        break;
    case Ellipse:
        painter.drawEllipse(QRect(lastPoint, endPoint));
        break;
    default:
        break;
    }

    update();
}

void MainWindow::on_actionNew_triggered() {
    if (maybeSave()) {
        clearImage();
        currentFile.clear();
    }
}

void MainWindow::on_actionOpen_triggered() {
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open Image"), "",
                                                        tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
        if (!fileName.isEmpty()) {
            loadImage(fileName);
        }
    }
}

void MainWindow::on_actionSave_triggered() {
    if (currentFile.isEmpty()) {
        on_actionSaveAs_triggered();
    } else {
        QString extension = QFileInfo(currentFile).suffix().toLower();
        if (extension == "jpg" || extension == "jpeg") {
            saveImage(currentFile, "JPEG");
        } else if (extension == "png") {
            saveImage(currentFile, "PNG");
        } else if (extension == "bmp") {
            saveImage(currentFile, "BMP");
        } else {
            saveImage(currentFile, "PNG");
        }
    }
}

void MainWindow::on_actionSaveAs_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Image"), "",
                                                    tr("PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)"));

    if (!fileName.isEmpty()) {
        if (fileName.endsWith(".png", Qt::CaseInsensitive)) {
            saveImage(fileName, "PNG");
        } else if (fileName.endsWith(".jpg", Qt::CaseInsensitive) ||
                   fileName.endsWith(".jpeg", Qt::CaseInsensitive)) {
            saveImage(fileName, "JPEG");
        } else if (fileName.endsWith(".bmp", Qt::CaseInsensitive)) {
            saveImage(fileName, "BMP");
        } else {
            fileName += ".png";
            saveImage(fileName, "PNG");
        }
    }
}

void MainWindow::on_actionExit_triggered() {
    close();
}

void MainWindow::on_actionPencil_triggered() {
    currentTool = Pencil;
}

void MainWindow::on_actionLine_triggered() {
    currentTool = Line;
}

void MainWindow::on_actionRectangle_triggered() {
    currentTool = Rectangle;
}

void MainWindow::on_actionEllipse_triggered() {
    currentTool = Ellipse;
}

void MainWindow::on_actionColor_triggered() {
    QColor newColor = QColorDialog::getColor(penColor, this, tr("Select Pen Color"));
    if (newColor.isValid()) {
        penColor = newColor;
    }
}

void MainWindow::on_actionPenWidth_triggered() {
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Pen Width"),
                                        tr("Select pen width:"),
                                        penWidth, 1, 50, 1, &ok);
    if (ok) {
        penWidth = newWidth;
    }
}
