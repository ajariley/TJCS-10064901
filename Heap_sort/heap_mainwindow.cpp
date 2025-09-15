#include "heap_mainwindow.h"
#include "ui_heap_mainwindow.h"
#include "heap_visual.h"
#include <Qdebug>
#include <QMenuBar>
#include <QToolBar>
#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QLabel>
#include <QIntValidator>
#include <QSplitter>
#include <QGraphicsView>

Heap_MainWindow::Heap_MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Heap_MainWindow)
{
    ui->setupUi(this);



    resize(600,400);
    isSorting = false;


    //添加菜单栏
    QMenuBar *menu = new QMenuBar(this);
    setMenuBar(menu);
    QMenu * startmenu = menu->addMenu("开始");
    QAction * docmenu = new QAction("帮助文档",this);
    menu->addAction(docmenu);


    //右侧工具面板
    QDockWidget *dock = new QDockWidget("操作区", this);
    dock->setAllowedAreas(Qt::RightDockWidgetArea);
    QWidget *dockWidget = new QWidget(dock);
    QVBoxLayout *dockLayout = new QVBoxLayout(dockWidget);

    QLabel *inputLabel = new QLabel("输入关键字：", dockWidget);
    inputEdit = new QLineEdit(dockWidget);
    inputEdit->setValidator(new QIntValidator(0, 999, inputEdit));//只能输入0-999的整数
    QString s = inputEdit->text();
    insertBtn = new QPushButton("插入", dockWidget);
    startBtn = new QPushButton("开始排序", dockWidget);
    deleteBtn = new QPushButton("删除", dockWidget);
    clearBtn = new QPushButton("清空", dockWidget);

    dockLayout->addWidget(inputLabel);
    dockLayout->addWidget(inputEdit);
    dockLayout->addWidget(insertBtn);
    dockLayout->addWidget(startBtn);
    dockLayout->addWidget(deleteBtn);
    dockLayout->addWidget(clearBtn);
    dockLayout->addStretch(); // 占位，把按钮挤到上面

    dockWidget->setLayout(dockLayout);
    dock->setWidget(dockWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    // 添加工具栏
    // QToolBar * toolbar = new QToolBar(this);
    // addToolBar(Qt::RightToolBarArea,toolbar);

    // //为工具栏加上按钮
    // QPushButton * startbtn = new QPushButton("准备开始",this);
    // toolbar->addWidget(startbtn);
    // toolbar->addSeparator();

    // 下方添加提示信息区域
    QLabel *labelPrefix = new QLabel("提示信息：", this);
    QLabel *labelMsg = new QLabel("请按照顺序从上至下操作，删除流程中途无法插入，一次删除正在进行时您无法点击清空按键", this);

    statusBar()->addPermanentWidget(labelPrefix);
    statusBar()->addPermanentWidget(labelMsg, 1);

    // 关键：在主窗口中创建画图容器
    QFrame *drawingContainer = new QFrame(this);
    drawingContainer->setFrameStyle(QFrame::Box | QFrame::Raised);
    drawingContainer->setLineWidth(2);

    QVBoxLayout *containerLayout = new QVBoxLayout(drawingContainer);
    containerLayout->setContentsMargins(0, 0, 0, 0);

    // 创建QGraphicsView作为画布（这是heap_visual需要的）
    QGraphicsView *graphicsView = new QGraphicsView(drawingContainer);
    containerLayout->addWidget(graphicsView);


    // 1. 实例化“舞台” (QGraphicsScene)
    //    'this' 参数确保了当主窗口关闭时，scene会被自动销毁，避免内存泄漏。
    QGraphicsScene *scene = new QGraphicsScene(this);

    // 2. 告诉“相机”(graphicsView) 要看哪个“舞台”(scene)
    //    这是最关键的连接步骤。
    graphicsView->setScene(scene);

    setCentralWidget(drawingContainer);

    //实例化一给heapcontroller对象
    heapcontroller = new HeapController(scene, this);

    // 信号槽
    connect(docmenu, &QAction::triggered, [=](){
        //1、创建一个对话框
        QDialog * helpdlg = new QDialog(this);
        helpdlg->setWindowTitle("帮助文档"); // 设置一个清晰的窗口标题

        helpdlg->setFixedWidth(800);
        helpdlg->show();
        helpdlg->setAttribute(Qt::WA_DeleteOnClose);

        //2、创建一个垂直的布局管理器
        QVBoxLayout* layout = new QVBoxLayout(helpdlg);

        //3、用标签来创建内容
        QLabel* usagelable = new QLabel(helpdlg);
        usagelable->setText(
            "<h1><b>操作指南</b></h1>"
            "<h3><b>1、插入节点 </b></h3>"
            "<p>在右侧“输入关键字”框中输入0-999的数字，然后点击“插入”按钮。注意一旦开始删除，您将"
            "<span style=\"font-size: 4.2em; font-weight: bold; color: #FF69B4;\">无法</span>"
            "点击插入，这会破坏原本已经找到的最大值</p>"

            "<h3><b>2、开始排序 </b></h3>"
            "<p>点击开始排序按钮。</p>"

            "<h3><b>3、逐步筛选 </b></h3>"
            "<p>点击删除按钮 每次按下按钮，都会自动把最大值放到后面，然后重新排列使得目前最大值放到根节点位置。删除过程进行中，您将"
            "<span style=\"font-size: 4.2em; font-weight: bold; color: #FF69B4;\">无法</span>"
            "点击清空，这会破坏正在进行的进程</p>"

            "<h3><b>4、清空 </b></h3>"
            "<p>点击清空按钮 按下按钮后，全部清空，您可以从头开始插入、排序、筛选。</p>"


            );
        usagelable->setWordWrap(true);//放置超过对话框边界
        layout->addWidget(usagelable);


        QFrame* separator = new QFrame(helpdlg);
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Sunken);//阴影效果
        layout->addWidget(separator);

        QLabel* introlabel = new QLabel(helpdlg);
        introlabel->setText(
            "<h1><b>堆排序介绍</b></h1>"
            "<p><b>堆排序是一种高效的、基于比较的排序算法。它利用了“堆”这种数据结构的特性，整个过程分为两个核心阶段 </b></p>"
            "<h3><b>1、建立堆 </b></h3>"
            "<p>首先，将待排序的无序序列在逻辑上构建成一个“大根堆”（本例是大根堆）。大根堆的特性是，根节点（顶端元素）永远是所有元素中最大的。我们利用了这个特性来完成后续筛选 </p>"

            "<h3><b>2、逐重复以下操作直到所有都被排列 </b></h3>"

            "<ul>"
            "<li>将堆顶的最大元素与堆末尾的元素交换。</li>" // 第一个项目
            "<li>将交换到末尾的元素视为“已排序”，并将堆的逻辑尺寸减一。</li>" // 第二个项目
            "<li>对新的堆顶元素执行“下滤”操作，使其在缩小的堆中找到正确位置，以重新维持大根堆的性质。</li>" // 第三个项目
            "</ul>"
            );
        introlabel->setWordWrap(true);//放置超过对话框边界
        layout->addWidget(introlabel);


    });

    // 相应数组变化，一般出现在插入后面
    connect(heapcontroller, &HeapController::heapChanged, this, [=](int val, int newSize){
        QString msg;
        if(newSize<=10){
            msg = QString("用户插入了数字 %1，现在堆中有 %2 个节点")
                              .arg(val)
                              .arg(newSize);
        }
        else if(newSize==11){
            msg = QString("用户插入数字 %1失败，最多只能插入10个节点！现在堆中有 10 个节点")
                              .arg(val)
                              ;
        }
        else if(newSize==qingkong){
            msg = QString("已清空！现在可以重新插入开始");
        }
        else if(newSize==zhengzaipaixu){
            msg = QString("正在对堆的有效内容排序");
        }
        else if(newSize==shouweijiaohuan){
            msg = QString("堆顶是最大值，放到最后，同时最后一个替换到堆顶。正在进行首尾交换操作");
        }

        labelMsg->setText(msg);
    });

    // 相应数组变化，一般出现在插入后面
    connect(heapcontroller, &HeapController::sortedDataUpdated, this, [=](const QString& sortedText){


        labelMsg->setText(sortedText);
    });

    // 点击插入信号，更新到堆里面同时状态栏播报
    connect(insertBtn, &QPushButton::clicked, this, [=]()mutable{
        QString text = inputEdit->text();
        if (text.isEmpty()) return; //处理错误

        int val = text.toInt();
        //  heap.insert(val);

        // inputEdit->clear();  这里清空导致后面读不到提前结束
        this->on_insertSimpleButton_clicked();

    });

    connect(insertBtn, &QPushButton::clicked,
            this, &Heap_MainWindow::on_insertSimpleButton_clicked);

    // 快捷键
    // connect(inputEdit, &QLineEdit::returnPressed, insertBtn, &QPushButton::clicked);
    connect(inputEdit, &QLineEdit::returnPressed, this, [=](){
        insertBtn->click();
    });

    // 点击开始排序，开始排序为大根堆
    connect(startBtn, &QPushButton::clicked, this, &Heap_MainWindow::on_startsortButton_clicked);

    // 点击删除，讲第一个移到最后，并排序
    connect(deleteBtn, &QPushButton::clicked, this, &Heap_MainWindow::on_deleteSimpleButton_clicked);

    // 点击清空，
    connect(clearBtn, &QPushButton::clicked, this, [=]()mutable{
        qDebug()<<"开始删除";
        this->on_clearButton_clicked();
        QString msg;
        msg = QString("");
        labelMsg->setText(msg);
        qDebug()<<"删除结束";

    });





}


Heap_MainWindow::~Heap_MainWindow()
{
    delete ui;
}

void Heap_MainWindow::on_insertSimpleButton_clicked()
{
    qDebug()<<"开始进入了heapmainwindow输入部分";
    // 获取输入的值
    QString text = inputEdit->text();
    if(text.isEmpty()) return;


    int value = text.toInt();
    qDebug()<<"开始进入了heapmainwindow输入部分，但是在heapcontroller前面";
    heapcontroller->simpleinsert(value);
    inputEdit->clear();
    qDebug()<<"已经进入了heapmainwindow输入部分";

}

void Heap_MainWindow::on_startsortButton_clicked()
{
    qDebug()<<"开始进入了heapmainwindow排序部分";
    heapcontroller->startHeapSort();
    qDebug()<<"已经进入了heapmainwindow排序部分";

}

void Heap_MainWindow::on_deleteSimpleButton_clicked()
{

    clearBtn->setEnabled(false);
    insertBtn->setEnabled(false);

    // A static variable to check if sorting has started.
    // isSorting = false;
    if (!isSorting) {
        qDebug()<<"开始进入了删除的首轮初始化部分";

        heapcontroller->startHeapSort(); //自动来一遍上滤
        heapcontroller->deletePrerequisite();
        isSorting = true;
    }

    // doNextHeapSortStep performs the swap and sift, which is what you want.
    heapcontroller->doNextHeapSortStep();
    // if (!heapcontroller->doNextHeapSortStep()) {
    //     isSorting = false; // Reset when sorting is finished
    // }
    qDebug()<<"结束了heapmainwindow删除部分";
    clearBtn->setEnabled(true);


}


void Heap_MainWindow::on_clearButton_clicked()
{
    isSorting = false;
    //按键重置
    heapcontroller->clearAll();
    insertBtn->setEnabled(true);

}

