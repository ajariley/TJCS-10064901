#ifndef HEAP_VISUAL_H
#define HEAP_VISUAL_H

#include <QWidget>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <vector>
#define qingkong 11111
#define zhengzaipaixu 22222
#define shouweijiaohuan 33333

const QBrush PINK_BRUSH = QBrush(QColor(255, 192, 203));

using namespace std;


struct HeapNode{
    HeapNode *parent;   //父节点`
    QPointF position;    //位置
    QString data;       //值
    int index;          //序号

    //指向图像项目的指针
    QGraphicsEllipseItem * circleItem = nullptr;
    QGraphicsTextItem * textItem = nullptr;


    //构造函数
    HeapNode(QString dt, int idx, HeapNode *pr):data(dt),index(idx),parent(pr){}


};

class Heap : public QObject
{
    Q_OBJECT
public:
    explicit Heap(QGraphicsScene *scene, QObject *parent = nullptr);
    ~Heap();

    /**
     * @brief Clears the old heap and builds a new one from input values.
     * @param values A vector of integers to build the heap from.
     */
    void buildAndDrawHeap(const std::vector<int>& values);

    friend class HeapController;
signals:
    void sortedDataUpdated(const QString& sortedText); //把排序号的部分发送出去
    // 定义一个信号，当节点被插入后发射
    void heapChanged(int newNodeValue, int newTotalSize);

protected:
    // Requirement 1: Position Calculation Function
    /*
    * 这里返回值是QPoint表示qt中内置的坐标格式
    * 参数为自己序号（判断相对位置）和总体序号（判断格式）
    */
    void Delay_MSec(unsigned int msec);

    QPointF calculatePosition(int index, int totalNodes);

    // Requirement 2: Drawing Primitive Functions
    /*
    * @brief Draws a single node (circle and number).
    * @param  节点和颜色
    */
    void drawNode(HeapNode& node, const QBrush& brush = Qt::lightGray);

    // Requirement 2: Drawing Primitive Functions 连接
    /*
    * @brief Draws a connection (rod/line) between two nodes.
    * @param  参数是两个节点
    */
    void drawConnection(const HeapNode& childNode, const HeapNode& parentNode);

    // Requirement 3: Main Drawing Function
    /*
    * @brief Draws the entire heap structure (all nodes and all connections).
    * @param  无参，所以需要能够访问所有的节点和参数总数 颜色也不用考虑，因为当然是默认颜色
    */
    void drawEntireHeap();

    // Requirement 4: Highlighting Function
    /*
    * @brief 改变某一个节点的颜色
    * @param 节点 颜色
    */
    void highlightNode(int index, const QBrush& brush = Qt::yellow);

    // Requirement 5: 简单插入和删除 handle the logic and animation
    void deleteTopNodeSimple();

    // Requirement 5: 简单插入和删除 handle the logic and animation
    void insertNodeSimple(int value);

    // Requirement 6: 一次性创建最大堆
    void buildMaxHeap();

    // Requirement 7: step-by-step 筛选套组
    void startSiftDown(int startIndex = 0);
    bool doNextSiftDownStep();
    void siftDown(int startIndex, int maxIndex);


private:
    vector<HeapNode> m_heapNodes;
    vector<int> m_sortedData; //  存储已经排序好的内容
    int m_heapSortSize; // 存储逻辑上一共有几个

    // 清空所有
    void clearGraphics();
    void clearData();
    void siftDown(int startIndex);


    QGraphicsScene *m_scene;

    // 绘图常量
    const int NODE_RADIUS = 25;
    const int HORIZONTAL_SPACING = 30;
    const int VERTICAL_SPACING = 50;
    const int CENTERED_TOP_MARGIN = 60;  // 顶层间隙，居中型
    const int COMPACT_TOP_MARGIN = 30;   // 顶层间隙，紧凑型



signals:
};

#endif // HEAP_VISUAL_H
