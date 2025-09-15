#include "heap_visual.h"
#include <cmath>  //为了计算位置
#include <QVBoxLayout>
#include <QTimer>
#include <QEventLoop>

using namespace std;

Heap::Heap(QGraphicsScene *scene, QObject *parent)
    : QObject(parent), m_scene(scene)
{
    //没有什么要构造的，除了传递的两个参数
}

Heap::~Heap()
{
    //
}

void Heap::Delay_MSec(unsigned int msec)

{

    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出

}
// Requirement 1: Position Calculation Function
QPointF Heap::calculatePosition(int index, int totalNodes){
    //先检验错误
    if (totalNodes <= 0) return QPointF(0, 0);

    // 居中型：永远都不会被运行

    if(totalNodes <=0){
        switch(index){
        case 0: return QPointF(0, CENTERED_TOP_MARGIN-NODE_RADIUS);    // root节点在顶部
        case 1: return QPointF(-60, CENTERED_TOP_MARGIN+80);            // left child
        case 2: return QPointF(60, CENTERED_TOP_MARGIN+80);             // right child
        default: return QPointF(0, 0);
        }
    }

    // 紧凑型
    else if(totalNodes>0 & totalNodes<=10){
        int level = floor(log2(index + 1));         //比如说0就是0层
        int nodesInLevel = pow(2, level);           //当前层的节点容量，比如0层1个
        int firstIndexOfLevel = nodesInLevel - 1;   //首节点的Index比如说第1层有两个，所以第一个节点是1
        int indexInLevel = index - firstIndexOfLevel;//节点在层中的序号，也是从0开始

        double y = level * (2 * NODE_RADIUS + VERTICAL_SPACING) + COMPACT_TOP_MARGIN;                     //最后不加一个半径是因为计算圆心的位置

        double centerX = 0;  // 假设画布中心x坐标为0（可根据实际画布宽度调整）
        double levelWidth = nodesInLevel * (2 * NODE_RADIUS) + (nodesInLevel - 1) * HORIZONTAL_SPACING; //总宽度
        double x = centerX - (levelWidth / 2.0) + (indexInLevel) * (2 * NODE_RADIUS + HORIZONTAL_SPACING);//最后不加一个半径因为计算左上角

        return QPointF(x,y);
    }
    else{
        return QPointF(0,0);
    }

}

// Requirement 2: Drawing Primitive Functions ：Node & Connection
void Heap::drawNode(HeapNode& node, const QBrush& brush) {
    // 如果已经存在了，首先移除
    if (node.circleItem) { delete node.circleItem; }
    if (node.textItem)   { delete node.textItem; }

    // 画圈
    qreal x = node.position.x() - NODE_RADIUS;
    qreal y = node.position.y() - NODE_RADIUS;
    node.circleItem = m_scene->addEllipse(x, y, 2 * NODE_RADIUS, 2 * NODE_RADIUS, QPen(Qt::black), brush); //m_scene是接到主窗口中的scene的关键
    node.circleItem->setZValue(1);//表示层级是1，比较高

    // 填充数字
    node.textItem = m_scene->addText(node.data);
    QRectF textRect = node.textItem->boundingRect();    //获取矩形框的尺寸
    node.textItem->setPos(node.position.x() - textRect.width()/2, node.position.y() - textRect.height()/2);
    node.textItem->setZValue(2); // Ensure text is on top of circles

}


void Heap::drawConnection(const HeapNode& childNode, const HeapNode& parentNode){
    if(m_heapNodes.size()==7){
        int m = 9;
    }
    QGraphicsLineItem* line = m_scene->addLine(
        childNode.position.x(), childNode.position.y(),
        parentNode.position.x(), parentNode.position.y(),
        QPen(Qt::black)
        );
    line->setZValue(0);//确保隐藏在最后，是最下面的层
    qDebug() << QString("坐标是（%1, %2）和(%3, %4)").arg(childNode.position.x()).arg(childNode.position.y()).arg(parentNode.position.x()).arg(parentNode.position.y());}


// Requirement 3: Main Drawing Function
void Heap::drawEntireHeap(){
    //先画连接，
    for(const auto& node : m_heapNodes){
        if(node.parent != nullptr){
            drawConnection(node, *node.parent);
        }
    }
    //再画节点
    for(auto& node : m_heapNodes){
        drawNode(node);
    }

}

// Requirement 4: Highlighting Function
void Heap::highlightNode(int index, const QBrush& brush){
    if(index >= 0 && index < m_heapNodes.size()){ //防御性检查，查看是否是有效索引
        HeapNode& node = m_heapNodes[index];      //通过别名操作原始节点
        QBrush oldbrush = node.circleItem->brush();//查看原来的颜色
        if(node.circleItem){
            //闪一下
            if (node.circleItem) {
                qDebug() << "old brush color:"
                         << oldbrush.color().name();
            }
            Delay_MSec(500);
            node.circleItem->setBrush(brush);
            Delay_MSec(500);
            node.circleItem->setBrush(oldbrush);
            Delay_MSec(500);
            node.circleItem->setBrush(brush);
            Delay_MSec(500);

            if (node.circleItem) {
                qDebug() << "Actual brush color:"
                         << brush.color().name();
            }
        }

    }
}

// Requirement 5: 插入一个节点到最后 最多10个
void Heap::insertNodeSimple(int value){
    qDebug()<<"已经进入了heap插入部分";
    if (m_heapNodes.size()>=1){
        int t = 0;
    }

    if(m_heapNodes.size()<10){
        HeapNode * parent  = m_heapNodes.empty() ? nullptr : &m_heapNodes[(m_heapNodes.size() - 1) / 2];
        int parentIndex = m_heapNodes.empty() ? -1 : (m_heapNodes.size() - 1) / 2;
        m_heapNodes.emplace_back(QString::number(value), m_heapNodes.size(), nullptr);       // emplace_back是vector直接插入的意思，这里是直接构造然后插入到最后，先统一设置为nullptr

        // !!!emplace_back导致内存重新分配！原本的parent指向错误的位置
        // 插入后再正确设置parent指针
        if (parentIndex != -1) {
            m_heapNodes.back().parent = &m_heapNodes[parentIndex];
        }

        // 先计算位置
        m_heapNodes[m_heapNodes.size()-1].position = calculatePosition(m_heapNodes.size()-1, m_heapNodes.size());

        //然后画圈和线
        drawNode(m_heapNodes[m_heapNodes.size()-1]);
        HeapNode& parentNode = *m_heapNodes.back().parent;  // 解引用获取对象
        // HeapNode& parentNode = m_heapNodes[(m_heapNodes.size() - 1) / 2];
        \
            if(parent!=nullptr){
            drawConnection(m_heapNodes[m_heapNodes.size()-1], parentNode);

        }

        emit heapChanged(value, m_heapNodes.size());
    }
    else{
        emit heapChanged(value, m_heapNodes.size()+1);
    }

}


// 这个函数不再使用
void Heap::deleteTopNodeSimple() {
    if (m_heapNodes.empty()) {
        return; // Nothing to delete
    }


    // 如果只有一个，只需要删除即可
    if (m_heapNodes.size()==1){
        clearGraphics();
    }

    // 1、获取最后一个节点的信息
    HeapNode lastNode = m_heapNodes.back();
    QString replacementData = lastNode.data;

    // 2、擦除最后一个节点
    if (lastNode.circleItem){
        m_scene->removeItem(lastNode.circleItem);
        delete lastNode.circleItem;
    }
    if (lastNode.textItem){
        m_scene->removeItem(lastNode.textItem);
        delete lastNode.textItem;
    }

    // 3、从vector中擦去
    m_heapNodes.pop_back();

    // 4、替换
    HeapNode& topNode = m_heapNodes[0];
    topNode.data = replacementData;

    // 5. Redraw the entire tree in its new, smaller shape.
    for(size_t i = 0; i < m_heapNodes.size(); ++i) {
        m_heapNodes[i].position = calculatePosition(i, m_heapNodes.size());
    }
    drawEntireHeap();
}

// Requirement 6: 完成一个大根堆
void Heap::buildMaxHeap() {
    qDebug()<<"已经进入了heap全排列部分";
    emit heapChanged(0,zhengzaipaixu);
    int lastNonLeafIndex = (m_heapNodes.size() / 2) - 1;
    for (int i = lastNonLeafIndex; i >= 0; --i) {
        siftDown(i, m_heapNodes.size());
    }
    // drawEntireHeap(); // Redraw after building
}


// Requirement 7: 下滤套组
// 当按下delete 键
void Heap::startSiftDown(int startIndex){
    if (m_heapNodes.empty()) return;

    m_sortedData.clear();
    m_heapSortSize = m_heapNodes.size(); // Initialize heap size for sorting
    qDebug() << "已经进入了heap准备部分，heapsortsize的值为" << m_heapSortSize;

    emit sortedDataUpdated("Sorted: []");

}

// 这个函数只有在删除下才会被调用
bool Heap::doNextSiftDownStep(){
    qDebug() << "已经进入了heap下滤部分，当前要分类的还有" << m_heapSortSize << "个";

    if (m_heapSortSize <= 0){
        QString finalStr = "筛选已结束 您也可以从上至下、从左至右读这棵树 Sorted: [";
        for (size_t i = 0; i < m_sortedData.size(); ++i) {
            finalStr += QString::number(m_sortedData[i]) + (i == m_sortedData.size() - 1 ? "" : ", ");
        }
        finalStr += "]";
        emit sortedDataUpdated(finalStr);
        return false;
    }
    if (m_heapSortSize == 1) {
        // 将最后一个也更新为绿色
        m_heapNodes[0].circleItem->setBrush(PINK_BRUSH);
        m_sortedData.insert(m_sortedData.begin(), m_heapNodes[0].data.toInt());

        // 2、 最后一个值插入到sorted 3、类对象
        m_heapSortSize--;



        QString finalStr = "这是您本轮最后一次有效点击 Sorted: [";
        for (size_t i = 0; i < m_sortedData.size(); ++i) {
            finalStr += QString::number(m_sortedData[i]) + (i == m_sortedData.size() - 1 ? "" : ", ");
        }
        finalStr += "]";
        emit sortedDataUpdated(finalStr);
        return false;


    }
    // 1、首尾交换
    emit heapChanged(0,shouweijiaohuan);
    highlightNode(m_heapSortSize - 1, Qt::yellow);
    highlightNode(0, Qt::yellow);
    swap(m_heapNodes[0].data, m_heapNodes[m_heapSortSize - 1].data);
    m_heapNodes[0].textItem->setPlainText(m_heapNodes[0].data);
    m_heapNodes[m_heapSortSize - 1].textItem->setPlainText(m_heapNodes[m_heapSortSize - 1].data);
    m_heapNodes[0].circleItem->setBrush(Qt::lightGray);
    m_heapNodes[m_heapSortSize - 1].circleItem->setBrush(Qt::lightGray);


    // 2、 最后一个值插入到sorted
    m_sortedData.insert(m_sortedData.begin(), m_heapNodes[m_heapSortSize - 1].data.toInt()); // insert操作下剩下的内容会向后顺移
    m_heapNodes[m_heapSortSize-1].circleItem->setBrush(PINK_BRUSH);
    // highlightNode(0, Qt::black);

    // 3、更新类对象
    m_heapSortSize--;

    // 4、更新label
    QString sortedStr = "Sorted: [";
    for (size_t i = 0; i < m_sortedData.size(); ++i) {
        sortedStr += QString::number(m_sortedData[i]) + (i == m_sortedData.size() - 1 ? "" : ", ");
    }
    sortedStr += "]";
    emit sortedDataUpdated(sortedStr);

    // 5、对剩余的内容进行排列
    if (m_heapSortSize > 0) {
        siftDown(0, m_heapSortSize);
    }




    return true;
    }

void Heap::siftDown(int startIndex, int maxIndex){

    // 循环，从第一个节点开始作为根节点
    int currentIndex = startIndex;
    // 直到某一次的查找发现和孩子节点是正确的，因为后面一定也是正确的，所以这一支可以退出循环了
    while(true){
        // 先计算孩子节点并且记录最大值
        int leftChildIndex = 2 * currentIndex + 1; //因为我们的节点序号从0开始
        int rightChildIndex = 2 * currentIndex + 2;
        int largestChildIndex = currentIndex;

        // 比较
        if (leftChildIndex < maxIndex && m_heapNodes[leftChildIndex].data.toInt() > m_heapNodes[largestChildIndex].data.toInt()) {
            largestChildIndex = leftChildIndex;
        }
        if (rightChildIndex < maxIndex && m_heapNodes[rightChildIndex].data.toInt() > m_heapNodes[largestChildIndex].data.toInt()) {
            largestChildIndex = rightChildIndex;
        }

        if (largestChildIndex != currentIndex) {
            highlightNode(currentIndex ,Qt::green);
            highlightNode(largestChildIndex ,Qt::green);

            std::swap(m_heapNodes[currentIndex].data, m_heapNodes[largestChildIndex].data);
            drawNode(m_heapNodes[currentIndex]);
            drawNode(m_heapNodes[largestChildIndex]);

            currentIndex = largestChildIndex;
        } else {
            break;
        }


    }
}







void Heap::clearGraphics() {
    m_scene->clear();
    qDebug()<<"删除图像";


}
void Heap::clearData(){
    m_heapSortSize = 0;
    m_heapNodes.clear();
    m_sortedData.clear();
    qDebug()<<"删除数据";
    emit heapChanged(0,qingkong);
}



