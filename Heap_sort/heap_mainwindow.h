#ifndef HEAP_MAINWINDOW_H
#define HEAP_MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include "heapcontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Heap_MainWindow;
}
QT_END_NAMESPACE

class Heap_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QLineEdit *inputEdit;
    QPushButton *insertBtn;
    QPushButton *startBtn;
    QPushButton *deleteBtn;
    QPushButton *clearBtn;

    Heap_MainWindow(QWidget *parent = nullptr);
    ~Heap_MainWindow();

private slots:
    void on_insertSimpleButton_clicked();
    void on_startsortButton_clicked();
    void on_deleteSimpleButton_clicked();
    void on_clearButton_clicked();


private:
    Ui::Heap_MainWindow *ui;
    HeapController * heapcontroller;
    bool isSorting;
};
#endif // HEAP_MAINWINDOW_H
