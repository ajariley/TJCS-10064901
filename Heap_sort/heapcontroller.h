#ifndef HEAPCONTROLLER_H
#define HEAPCONTROLLER_H

#include <QObject>
#include <QString>

class Heap;
class QGraphicsScene;


/**
 * @brief The HeapController class
 * 这是一个控制类，作为UI（如Heap_MainWindow）和核心逻辑（Heap类）之间的桥梁。
 * 它接收来自UI的简单请求，并调用Heap类中的相应方法来执行操作。
 */
class HeapController : public QObject
{
    Q_OBJECT
public:
    explicit HeapController(QGraphicsScene* scene, QObject* parent = nullptr);
    ~HeapController();

    void simpleinsert(int value);
    void startHeapSort();
    bool doNextHeapSortStep();
    void clearAll();
    void deletePrerequisite();

signals:
    // 信号转发，从heap 中发射的信号经由heap_controller连接到自己的信号再连接到ui界面
    void sortedDataUpdated(const QString& sortedText);
    void heapChanged(int newNodeValue, int newTotalSize);


private:

    //这是heap_controller的核心： 管理heap对象
    Heap* m_heap;
};

#endif // HEAPCONTROLLER_H
