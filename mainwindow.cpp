#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include <QtDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    ui->lineEditresult->setText(tr("<drop content>"));

    event->acceptProposedAction();
    emit changed(event->mimeData());
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasText()) {
        ui->lineEditresult->setText(mimeData->text());
    }
    else {
        ui->lineEditresult->setText(tr("Cannot display data"));
    }
    event->acceptProposedAction();


}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{

    ui->lineEditresult->setText("");
    event->accept();
}

void MainWindow::on_pushButtonClear_clicked()
{
QString x;
x= "C:/Users/Jonathan/Documents/qt/a.mp3";

QByteArray ba = x.toLatin1();
  const char *c_str2 = ba.data();
  qDebug()<<c_str2;
    TagLib::FileRef f(c_str2);

    TagLib::Tag *tag = f.tag();

    ui->lineEditartist->setText(tag->artist().toCString());
}
