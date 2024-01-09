#ifndef ANIMATEDARROW_H
#define ANIMATEDARROW_H

#include <QObject>
#include <QWidget>
#include <QPointF>
#include <QPen>
#include <QLabel>
#include <QPainterPath>
#include <QPainter>
#include <QTimer>

class AnimatedArrow : public QObject
{
    Q_OBJECT

public:
    enum class TypeOfLine {
        STRAIGHT,
        DOTS,
        CURVES,
    };

    enum class TypeOfArrowhead {
        NONE,
        ARROW,
        BIGDOT,
        POINTY,
    };

    enum class TypeOfAnimation {
        NONE,
        GROW,
        FADEIN,
        FADEOUT,
        FADEINANDOUT,
        FADEFROMSTARTTOEND,
    };

    enum class TypeOfRepetition {
        NEVER,
        ONCE,
        CONTINUING
    };

    explicit AnimatedArrow(QWidget* widget, QPointF start, QPointF end, bool activeOnConstruction = true, QObject *parent = nullptr);
    ~AnimatedArrow();

    QWidget *parent() const;
    void setParent(QWidget *newParent);
    QPointF start() const;
    void setStart(QPointF newStart);
    QPointF end() const;
    void setEnd(QPointF newEnd);
    QPen pen() const;
    void setPen(const QPen &newPen);
    TypeOfLine typeOfLine() const;
    void setTypeOfLine(TypeOfLine newTypeOfLine);
    TypeOfArrowhead typeOfArrowhead() const;
    void setTypeOfArrowhead(TypeOfArrowhead newTypeOfArrowhead);
    TypeOfAnimation typeOfAnimation() const;
    void setTypeOfAnimation(TypeOfAnimation newTypeOfAnimation);
    TypeOfRepetition typeOfRepetition() const;
    void setTypeOfRepetition(TypeOfRepetition newTypeOfRepetition);
    unsigned int duration() const; // in msecs
    void setDuration(unsigned int newDuration); // in msecs
    double sizeOfArrowhead() const;
    void setSizeOfArrowhead(double newSizeOfArrowhead);
    double angleOfArrowhead() const;
    void setAngleOfArrowhead(double newAngleOfArrowhead);
    bool isActive() const;
    void activate(bool active = true);

signals:
    void stopAllTimers();

public slots:
    void parentResized(const QSize &newSize);
    void startAnimation();

    void setTypeOfLine(int index);
    void setTypeOfArrowhead(int index);
    void setTypeOfAnimation(int index);
    void setTypeOfRepetition(int index);

    void coordinatesChanged(QPointF start, QPointF end);

private:
    static constexpr unsigned int INTERVAL = 20;

    QWidget* m_parent;
    bool m_active;
    QSize m_size;
    QPointF m_start;
    QPointF m_end;
    QPointF m_endWithoutArrowhead;
    void calculateEndWithoutArrowhead(double* distanceToEnd = nullptr);
    QPen m_pen;
    TypeOfLine m_typeOfLine;
    TypeOfArrowhead m_typeOfArrowhead;
    double m_sizeOfArrowhead;
    double m_angleOfArrowhead;
    TypeOfAnimation m_typeOfAnimation;
    TypeOfRepetition m_typeOfRepetition;
    unsigned int m_duration; // in msecs

    QLabel* m_label;

    QPainterPath m_complexPath;
    QPainterPath calculateSimplePath(QPointF start, QPointF end);
    void calculateComplexPath(const QPainterPath &simplePath);
    QPainterPath calculateArrowhead(QPointF tip, double lineAngle, double lineWidth);

    unsigned int m_timeGone;

    void switchTimerRepetitionBehavior();
    bool isAnyTimerActive();

    QTimer* m_timerGrow;
    void timerGrowTimeOut();
    QTimer* m_timerFadeIn;
    void timerFadeInTimeOut();
    QTimer* m_timerFadeOut;
    void timerFadeOutTimeOut();
    QTimer* m_timerFadeInAndOut;
    void timerFadeInAndOutTimeOut();
    QTimer* m_timerFadeFromStartToEnd;
    void timerFadeFromStartToEndTimeOut();

};

#endif // ANIMATEDARROW_H
