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

    //This event will only take in text mimeData

    if (!(mimeData->hasText())) {
       ui->lineEditresult->setText(tr("Cannot display data"));
       return;
    }

    event->acceptProposedAction();

    //Next is to read the dropped file and make the appropriate updates to the gui.
    QString droppedFilePath(mimeData->text());
    droppedFilePath = droppedFilePath.replace("file:///", "");
    //QString x;

    //x= "C:/Users/Jonathan/Downloads/JPOPSUKI/01 Ready25.flac";

    QFileInfo fi(droppedFilePath);
    //Get the suffix of the file. To determine the encoding type later
    QString ext = fi.suffix();
    qDebug()<<ext;
    //Convert the QString of the path to the file to a c-string. Taglib only accepts a c-string in its constructor
    QByteArray ba = droppedFilePath.toLatin1();

    const char *c_str2 = ba.data();

    qDebug()<<c_str2;

    TagLib::FileRef f(c_str2);
    if(!f.isNull()){
        TagLib::Tag *tag = f.tag();

        ui->lineEditartist->setText(tag->artist().toCString());
        ui->lineEditname->setText(tag->album().toCString());
        //Check if a year of release exists. If it doesnt set the date field to empty
        if(!(QString::number(tag->year())=='0')){
            ui->lineEditdate->setText(QString::number(tag->year()));
        }else{
            ui->lineEditdate->setText("");
        }

        //Check the encoding type for the codec combobox
        if(ext == "mp3"){
            ui->comboBoxcodec->setCurrentIndex(2);
        }
        else if(ext == "FLAC" || ext == "flac"){
             qDebug()<<"ghjgfhj";
            TagLib::FLAC::File flcfile(c_str2);
            TagLib::FLAC::Properties flcprop(&flcfile);
            qDebug()<<flcprop.channels();
            //if(QString::number(flc.audioProperties()->bitsPerSample())=="24"){
                //ui->comboBoxcodec->setCurrentIndex(1);
           // }else
            //ui->comboBoxcodec->setCurrentIndex(0);
        }
        else if(ext == "m4a"){
            ui->comboBoxcodec->setCurrentIndex(4);

        }

    }


}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{

    ui->lineEditresult->setText("");
    event->accept();
}

void MainWindow::on_pushButtonClear_clicked()
{
    clear();
}

void MainWindow::clear(){
    ui->lineEditartist->setText("");
    ui->lineEditdate->setText("");
    ui->lineEditedition->setText("");
    ui->lineEditname->setText("");
    ui->lineEditresult->setText("");
    ui->comboBoxcodec->setCurrentIndex(0);
    ui->comboBoxsource->setCurrentIndex(0);
}
