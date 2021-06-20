#include "StreamMainWindow_ui.h"

#include "DisplayGLWidget.h"
#include "StreamMainWindow.h"

void StreamMainWindowUI::setupUI(StreamMainWindow *widget) {
  // cache main widget
  main_widget = widget;
  // set window title.
  main_widget->setWindowTitle("Stream");
  main_widget->setSizePolicy(QSizePolicy::Policy::Preferred,
                             QSizePolicy::Policy::Preferred);

  // setup UIs.
  setupMenus();
  setupWidgets();
}

void StreamMainWindowUI::setupMenus() {
  QMenu *option_menu = main_widget->menuBar()->addMenu("Option");

  QAction *exitAction =
      option_menu->addAction("Exit", qApp, &QCoreApplication::quit);
  exitAction->setShortcuts(QKeySequence::Quit);
}

void StreamMainWindowUI::setupWidgets() {
  QFrame *frame = new QFrame;
  frame->setMinimumSize(500, 500);

  QSplitter *spliter = new QSplitter(Qt::Orientation::Vertical, frame);
  spliter->setAutoFillBackground(true);
  spliter->setChildrenCollapsible(false);

  spliter->addWidget(setupDisplayWidget(frame));
  spliter->addWidget(setupInputWidget(frame));

  QHBoxLayout *layout = new QHBoxLayout(frame);
  layout->addWidget(spliter);

  main_widget->setCentralWidget(frame);
}

QWidget *StreamMainWindowUI::setupDisplayWidget(QWidget *parent) {
  display_glwidget = new DisplayGLWidget(parent);
  display_glwidget->setMinimumSize(200, 200);

  return display_glwidget;
}

QWidget *StreamMainWindowUI::setupInputWidget(QWidget *parent) {
  QWidget *widget = new QWidget(parent);
  widget->setMinimumSize(200, 100);

  QFrame *label_frame = new QFrame(widget);
  QVBoxLayout *label_layout = new QVBoxLayout(label_frame);
  label_layout->setAlignment(Qt::AlignmentFlag::AlignTop);
  QFrame *input_frame = new QFrame(widget);
  QVBoxLayout *input_layout = new QVBoxLayout(input_frame);
  input_layout->setAlignment(Qt::AlignmentFlag::AlignTop);

  QSplitter *splitter = new QSplitter(Qt::Orientation::Horizontal, widget);
  splitter->setChildrenCollapsible(false);
  splitter->addWidget(label_frame);
  splitter->setStretchFactor(0, 1);
  splitter->addWidget(input_frame);
  splitter->setStretchFactor(1, 3);
  splitter->addWidget(setupButtonWidget(widget));
  splitter->setStretchFactor(2, 2);

  const int text_height = 30;
  const QFont font("Arial", 18, QFont::Weight::Normal);

  // setup ip address ui
  QLabel *ip_label = new QLabel("ip");
  ip_label->setFixedHeight(text_height);
  ip_label->setFont(font);
  label_layout->addWidget(ip_label);
  ip_adress_line = new QLineEdit("127.0.0.1");
  ip_adress_line->setFixedHeight(text_height);
  ip_adress_line->setInputMask("000.000.000.000");
  ip_adress_line->setFont(font);
  input_layout->addWidget(ip_adress_line);

  // setup out stream port ui
  QLabel *stream_port_label = new QLabel("out port");
  stream_port_label->setFixedHeight(text_height);
  stream_port_label->setFont(font);
  label_layout->addWidget(stream_port_label);
  stream_port_line = new QLineEdit("10002");
  stream_port_line->setFixedHeight(text_height);
  stream_port_line->setValidator(new QIntValidator(0, 65535, stream_port_line));
  stream_port_line->setFont(font);
  input_layout->addWidget(stream_port_line);

  // setup listen stream port ui
  QLabel *listen_port_label = new QLabel("in port");
  listen_port_label->setFixedHeight(text_height);
  listen_port_label->setFont(font);
  label_layout->addWidget(listen_port_label);
  listen_port_line = new QLineEdit("10005");
  listen_port_line->setFixedHeight(text_height);
  listen_port_line->setValidator(new QIntValidator(0, 65535, listen_port_line));
  listen_port_line->setFont(font);
  input_layout->addWidget(listen_port_line);

  return widget;
}

QWidget *StreamMainWindowUI::setupButtonWidget(QWidget *parent) {
  QWidget *widget = new QWidget(parent);
  widget->setMinimumSize(100, 50);

  QVBoxLayout *layout = new QVBoxLayout(widget);
  layout->setAlignment(Qt::AlignmentFlag::AlignTop);

  const int text_height = 30;
  const QFont font("Arial", 18, QFont::Weight::Normal);

  capture_type_combo = new QComboBox();
  capture_type_combo->addItem("Webcam");
  capture_type_combo->addItem("Desktop");
  // connect select event to mainwidget.
  main_widget->connect(capture_type_combo, SIGNAL(currentIndexChanged(int)),
                       main_widget, SLOT(onSelectCaptureType(int)));
  layout->addWidget(capture_type_combo);

  QPushButton *start_btn = new QPushButton("Start Stream");
  start_btn->setFixedHeight(text_height);
  // connect button event to mainwidget.
  main_widget->connect(start_btn, SIGNAL(clicked()), main_widget,
                       SLOT(onStartButtonClicked()));
  layout->addWidget(start_btn);

  QPushButton *watch_btn = new QPushButton("Watch Stream");
  watch_btn->setFixedHeight(text_height);
  // connect button event to mainwidget.
  main_widget->connect(watch_btn, SIGNAL(clicked()), main_widget,
                       SLOT(onWatchButtonClicked()));
  layout->addWidget(watch_btn);

  QPushButton *leave_btn = new QPushButton("Leave");
  leave_btn->setFixedHeight(text_height);
  // connect button event to mainwidget.
  main_widget->connect(leave_btn, SIGNAL(clicked()), main_widget,
                       SLOT(onLeaveButtonClicked()));
  layout->addWidget(leave_btn);

  return widget;
}
