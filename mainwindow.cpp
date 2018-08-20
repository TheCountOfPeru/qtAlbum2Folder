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
    //Next is to read the dropped file and make the appropriate updates to the gui.
    QString droppedFilePath(mimeData->text());//text() returns a QString

    droppedFilePath = droppedFilePath.replace("file:///", "");
    //std::string droppedFilePaths = droppedFilePath.toStdString();
    wchar_t *path = new wchar_t[droppedFilePath.length() + 1];
    droppedFilePath.toWCharArray(path);
    path[droppedFilePath.length()] = 0;
    QFileInfo fi(droppedFilePath);
    //Get the suffix of the file. To determine the encoding type later
    QString ext = fi.suffix();
    //Convert the QString of the path to the file to a c-string. Taglib only accepts a c-string in its constructor
    //QByteArray ba = droppedFilePath.toLocal8Bit();

    //const char *c_str2 = ba.data();
    //droppedFilePath.toStdString().c_str();
    //ui->lineEditresult->setText(droppedFilePath.toStdString().c_str());
    TagLib::FileRef f(path);
    //TagLib::FileRef f(QFile::encodeName(droppedFilePath).constData(), true);
    //ui->lineEditresult->setText(QFile::encodeName(droppedFilePath).constData());
    if(!f.isNull()){

        TagLib::Tag *tag = f.tag();

        ui->lineEditartist->setText(QString::fromStdString(tag->artist().to8Bit(true)));//to8bit makes sure unicode is displayed
        ui->lineEditname->setText(QString::fromStdString(tag->album().to8Bit(true)));
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
             ui->comboBoxcodec->setCurrentIndex(0);
        }
        else if(ext == "m4a"){
            ui->comboBoxcodec->setCurrentIndex(4);

        }

    }
    else{
        qDebug() << "Unable to open the file!";
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

void MainWindow::on_pushButtonApply_clicked()
{
    QString result = "";
    result.append(ui->lineEditartist->text() + " - ");
    result.append(ui->lineEditname->text() + " ");
    result.append("("+ui->lineEditdate->text()+") ");
    result.append("["+ui->comboBoxsource->currentText());
    result.append(" "+ui->comboBoxcodec->currentText()+"]");
    ui->lineEditresult->setText(result);
}

void MainWindow::on_pushButtonCopy_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    //QString originalText = clipboard->text();
    clipboard->setText(ui->lineEditresult->text());
}
