#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qcombobox.h"
#include <QMainWindow>
#include <QResizeEvent>
#include <QSpinBox>
#include <QPen>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    QSpinBox* m_startXSpinBox;
    QSpinBox* m_startYSpinBox;
    QSpinBox* m_endXSpinBox;
    QSpinBox* m_endYSpinBox;
    QSpinBox* m_widthSpinBox;
    QComboBox* m_joinComboBox;
    QSpinBox* m_colorRSpinBox;
    QSpinBox* m_colorGSpinBox;
    QSpinBox* m_colorBSpinBox;

private slots:
    void startEndChanged();
    void penChanged();

signals:
    void parentResized(const QSize &newSize);
    void coordinatesChanged(QPointF start, QPointF end);
    void setPen(QPen pen = QPen());

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
};
#endif // MAINWINDOW_H
