#include "heapcontroller.h"
#include "heap_visual.h"
#include <QGraphicsScene>
#include <QDebug>


HeapController::HeapController(QGraphicsScene * scene,  QObject *parent)
    : QObject{parent}
{
    // Controller创建并拥有Heap对象。我们将scene传递给Heap的构造函数。
    // 将this作为Heap的父对象，这样当Controller被销毁时，Heap也会被自动销毁。
    m_heap = new Heap(scene, this);


    connect(m_heap, &Heap::sortedDataUpdated, this, &HeapController::sortedDataUpdated);
    // 将底层heap的信号连接到controller的信号上
    connect(m_heap, &Heap::heapChanged, this, &HeapController::heapChanged);
}


HeapController::~HeapController()
{
    // 由于m_heap的父对象是this，Qt的内存管理机制会自动删除m_heap，
    // 所以这里的析构函数可以是空的。
}

void HeapController::simpleinsert(int value)
{
    // 将UI的请求直接转发给Heap对象
    m_heap->insertNodeSimple(value);
    qDebug()<<"已经进入了heapcontroller插入部分";

}

void HeapController::startHeapSort()
{
    // 将UI的请求直接转发给Heap对象
    m_heap->buildMaxHeap();
    qDebug()<<"已经进入了heapcontroller排序部分";
}

void HeapController::deletePrerequisite()
{
    qDebug()<<"已经进入了heapcontroller删除准备部分";
    m_heap->startSiftDown(0);
}

bool HeapController::doNextHeapSortStep()
{
    qDebug()<<"即将进入heapcontroller删除部分";

    // 将UI的请求直接转发给Heap对象，并返回其结果
    return m_heap->doNextSiftDownStep();
}

void HeapController::clearAll()
{
    // 将UI的请求直接转发给Heap对象
    m_heap->clearGraphics();
    m_heap->clearData();
}
