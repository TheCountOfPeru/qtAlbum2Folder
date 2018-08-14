#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfo>
#include "taglib\fileref.h"
#include "taglib\tag.h"
#include "taglib\tpropertymap.h"
#include "taglib\flacfile.h"
class QMimeData;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void clear();

signals:
    void changed(const QMimeData *mimeData = nullptr);
private slots:
    void on_pushButtonClear_clicked();
};

#endif // MAINWINDOW_H
