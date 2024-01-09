#include "animatedarrow.h"

QPointF AnimatedArrow::start() const
{
    return m_start;
}

void AnimatedArrow::setStart(QPointF newStart)
{
    if (m_parent == nullptr) {
        m_start = QPointF(0,0);
        return;
    }
    else if (newStart == m_start) {
        return;
    }
    else {
        m_start.setX(newStart.x() > m_parent->width() ? m_parent->width() : newStart.x() < 0 ? 0 : newStart.x());
        m_start.setY(newStart.y() > m_parent->height() ? m_parent->height() : newStart.y() < 0 ? 0 : newStart.y());
        calculateEndWithoutArrowhead();
        if (!isAnyTimerActive()) startAnimation();
    }
}

QPointF AnimatedArrow::end() const
{
    return m_end;
}

void AnimatedArrow::setEnd(QPointF newEnd)
{
    if (m_parent == nullptr) {
        m_end = QPointF(0,0);
        return;
    }
    else if (newEnd == m_end) {
        return;
    }
    else {
        m_end.setX(newEnd.x() > m_parent->width() ? m_parent->width() : newEnd.x() < 0 ? 0 : newEnd.x());
        m_end.setY(newEnd.y() > m_parent->height() ? m_parent->height() : newEnd.y() < 0 ? 0 : newEnd.y());
        calculateEndWithoutArrowhead();
        if (!isAnyTimerActive()) startAnimation();
    }
}

QPen AnimatedArrow::pen() const
{
    return m_pen;
}

void AnimatedArrow::setPen(const QPen &newPen)
{
    if (newPen == m_pen) return;
    m_pen = newPen;
    calculateEndWithoutArrowhead();
    if (!isAnyTimerActive()) startAnimation();
}

AnimatedArrow::TypeOfLine AnimatedArrow::typeOfLine() const
{
    return m_typeOfLine;
}

void AnimatedArrow::setTypeOfLine(TypeOfLine newTypeOfLine)
{
    if (newTypeOfLine == m_typeOfLine) return;
    m_typeOfLine = newTypeOfLine;
    if (!isAnyTimerActive()) startAnimation();
}

void AnimatedArrow::setTypeOfLine(int index)
{
    switch (index) {
    case 0:
        setTypeOfLine(TypeOfLine::STRAIGHT);
        break;
    case 1:
        setTypeOfLine(TypeOfLine::DOTS);
        break;
    case 2:
        setTypeOfLine(TypeOfLine::CURVES);
        break;
    default:
        setTypeOfLine(TypeOfLine::STRAIGHT);
        break;
    }
}

AnimatedArrow::TypeOfArrowhead AnimatedArrow::typeOfArrowhead() const
{
    return m_typeOfArrowhead;
}

void AnimatedArrow::setTypeOfArrowhead(TypeOfArrowhead newTypeOfArrowhead)
{
    if (newTypeOfArrowhead == m_typeOfArrowhead) return;
    m_typeOfArrowhead = newTypeOfArrowhead;
    calculateEndWithoutArrowhead();
    if (!isAnyTimerActive()) startAnimation();
}

void AnimatedArrow::setTypeOfArrowhead(int index)
{
    switch (index) {
    case 0:
        setTypeOfArrowhead(TypeOfArrowhead::NONE);
        break;
    case 1:
        setTypeOfArrowhead(TypeOfArrowhead::ARROW);
        break;
    case 2:
        setTypeOfArrowhead(TypeOfArrowhead::BIGDOT);
        break;
    case 3:
        setTypeOfArrowhead(TypeOfArrowhead::POINTY);
        break;
    default:
        setTypeOfArrowhead(TypeOfArrowhead::NONE);
        break;
    }
}

AnimatedArrow::TypeOfAnimation AnimatedArrow::typeOfAnimation() const
{
    return m_typeOfAnimation;
}

void AnimatedArrow::setTypeOfAnimation(TypeOfAnimation newTypeOfAnimation)
{
    if (newTypeOfAnimation == m_typeOfAnimation) return;
    m_typeOfAnimation = newTypeOfAnimation;
    startAnimation();
}

void AnimatedArrow::setTypeOfAnimation(int index)
{
    switch (index) {
    case 0:
        setTypeOfAnimation(TypeOfAnimation::NONE);
        break;
    case 1:
        setTypeOfAnimation(TypeOfAnimation::GROW);
        break;
    case 2:
        setTypeOfAnimation(TypeOfAnimation::FADEIN);
        break;
    case 3:
        setTypeOfAnimation(TypeOfAnimation::FADEOUT);
        break;
    case 4:
        setTypeOfAnimation(TypeOfAnimation::FADEINANDOUT);
        break;
    case 5:
        setTypeOfAnimation(TypeOfAnimation::FADEFROMSTARTTOEND);
        break;
    default:
        setTypeOfAnimation(TypeOfAnimation::NONE);
        break;
    }
}
AnimatedArrow::TypeOfRepetition AnimatedArrow::typeOfRepetition() const
{
    return m_typeOfRepetition;
}

void AnimatedArrow::setTypeOfRepetition(TypeOfRepetition newTypeOfRepetition)
{
    if (newTypeOfRepetition == m_typeOfRepetition) return;
    m_typeOfRepetition = newTypeOfRepetition;
    if (!isAnyTimerActive()) startAnimation();
}

void AnimatedArrow::setTypeOfRepetition(int index)
{
    switch (index) {
    case 0:
        setTypeOfRepetition(TypeOfRepetition::NEVER);
        break;
    case 1:
        setTypeOfRepetition(TypeOfRepetition::ONCE);
        break;
    case 2:
        setTypeOfRepetition(TypeOfRepetition::CONTINUING);
        break;
    default:
        setTypeOfRepetition(TypeOfRepetition::NEVER);
        break;
    }
}

void AnimatedArrow::coordinatesChanged(QPointF start, QPointF end)
{
    setStart(start);
    setEnd(end);
}

void AnimatedArrow::calculateEndWithoutArrowhead(double* distanceToEnd)
{
    switch (m_typeOfArrowhead) {
    case TypeOfArrowhead::ARROW: {
        QLineF startToEnd(m_start, m_end);
        QLineF line1;
        line1.setP1(m_end);
        line1.setAngle(startToEnd.angle() - m_angleOfArrowhead);
        line1.setLength(m_pen.widthF() * m_sizeOfArrowhead);
        QLineF line2;
        line2.setP1(m_end);
        line2.setAngle(startToEnd.angle() + m_angleOfArrowhead);
        line2.setLength(m_pen.widthF() * m_sizeOfArrowhead);
        QLineF line3(line1.p2(), line2.p2());
        QPointF intersectionPoint;
        line3.intersects(startToEnd, &intersectionPoint);
        double distance = QLineF(m_end, intersectionPoint).length() + m_pen.widthF() / 2;
        startToEnd.setLength(startToEnd.length() - distance);
        m_endWithoutArrowhead = startToEnd.p2();
        if (distanceToEnd) *distanceToEnd = distance;
        break;
    }
    case TypeOfArrowhead::BIGDOT: {
        QLineF startToEnd(m_start, m_end);
        double distance = m_pen.widthF() * m_sizeOfArrowhead + m_pen.widthF() / 2;
        startToEnd.setLength(startToEnd.length() - distance);
        m_endWithoutArrowhead = startToEnd.p2();
        if (distanceToEnd) *distanceToEnd = distance;
        break;
    }
    default:
        m_endWithoutArrowhead = m_end;
        break;
    }
}

unsigned int AnimatedArrow::duration() const
{
    return m_duration;
}

void AnimatedArrow::setDuration(unsigned int newDuration)
{
    if (newDuration == m_duration || newDuration == 0) return;
    emit stopAllTimers();
    m_duration = newDuration;
    startAnimation();
}

void AnimatedArrow::parentResized(const QSize &newSize)
{
    if (m_size == newSize || newSize.isEmpty()) return;
    m_start.setX(1.0 * newSize.width() / m_size.width() * m_start.x());
    m_start.setY(1.0 * newSize.height() / m_size.height() * m_start.y());
    m_end.setX(1.0 * newSize.width() / m_size.width() * m_end.x());
    m_end.setY(1.0 * newSize.height() / m_size.height() * m_end.y());
    m_label->setGeometry(QRect(QPoint(0, 0), newSize));
    m_size = newSize;
    if (!isAnyTimerActive()) startAnimation();
}

void AnimatedArrow::startAnimation()
{
    emit stopAllTimers();
    m_timeGone = 0;
    switch (m_typeOfAnimation) {
    case TypeOfAnimation::NONE: {
        QPixmap pixmap(m_size);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setPen(m_pen);
        painter.setBrush(QBrush(m_pen.color()));
        painter.drawPath(calculateSimplePath(m_start, m_endWithoutArrowhead));
        painter.drawPath(calculateArrowhead(m_end, QLineF(m_start, m_end).angle(), m_pen.widthF()));
        m_label->setPixmap(pixmap);
        break;
    }
    case TypeOfAnimation::GROW: {
        m_timerGrow->start(INTERVAL);
        break;
    }
    case TypeOfAnimation::FADEIN: {
        m_timerFadeIn->start(INTERVAL);
        break;
    }
    case TypeOfAnimation::FADEOUT: {
        m_timerFadeOut->start(INTERVAL);
        break;
    }
    case TypeOfAnimation::FADEINANDOUT: {
        m_timerFadeInAndOut->start(INTERVAL);
        break;
    }
    case TypeOfAnimation::FADEFROMSTARTTOEND:
        calculateComplexPath(calculateSimplePath(m_start, m_endWithoutArrowhead));
        m_timerFadeFromStartToEnd->start(INTERVAL);
        break;
    }
}

bool AnimatedArrow::isActive() const
{
    return m_active;
}

void AnimatedArrow::activate(bool active)
{
    m_active = active;
}

double AnimatedArrow::angleOfArrowhead() const
{
    return m_angleOfArrowhead;
}

void AnimatedArrow::setAngleOfArrowhead(double newAngleOfArrowhead)
{
    m_angleOfArrowhead = newAngleOfArrowhead;
}

double AnimatedArrow::sizeOfArrowhead() const
{
    return m_sizeOfArrowhead;
}

void AnimatedArrow::setSizeOfArrowhead(double newSizeOfArrowhead)
{
    m_sizeOfArrowhead = newSizeOfArrowhead;
}

QPainterPath AnimatedArrow::calculateSimplePath(QPointF start, QPointF end)
{
    QPainterPath path;
    switch (m_typeOfLine) {
    case TypeOfLine::STRAIGHT: {
        path.moveTo(start);
        path.lineTo(end);
        break;
    }
    case TypeOfLine::DOTS:
        break;
    case TypeOfLine::CURVES:
        break;
    }
    return path;
}

void AnimatedArrow::calculateComplexPath(const QPainterPath &simplePath)
{
    m_complexPath = QPainterPath();
    unsigned int length = simplePath.length();
    QPointF nextPoint;
    m_complexPath.moveTo(m_start);
    for (unsigned int i = 1; i < length; ++i) {
        nextPoint = simplePath.pointAtPercent(1.0 * i / length);
        m_complexPath.lineTo(nextPoint);
    }
    m_complexPath.lineTo(m_end);
}

QPainterPath AnimatedArrow::calculateArrowhead(QPointF tip, double lineAngle, double lineWidth)
{
    QPainterPath path;
    switch (m_typeOfArrowhead) {
    case TypeOfArrowhead::NONE:
        break;
    case TypeOfArrowhead::ARROW: {
        QLineF line1;
        line1.setP1(tip);
        line1.setAngle(lineAngle - m_angleOfArrowhead);
        line1.setLength(lineWidth * m_sizeOfArrowhead);
        QLineF line2;
        line2.setP1(tip);
        line2.setAngle(lineAngle + m_angleOfArrowhead);
        line2.setLength(lineWidth * m_sizeOfArrowhead);

        path.moveTo(line1.p2());
        path.lineTo(tip);
        path.lineTo(line2.p2());
        path.lineTo(line1.p2());
        break;
    }
    case TypeOfArrowhead::BIGDOT: {
        path.addEllipse(tip, lineWidth * m_sizeOfArrowhead, lineWidth * m_sizeOfArrowhead);
        break;
    }
    case TypeOfArrowhead::POINTY:
        break;
    }
    return path;
}

void AnimatedArrow::switchTimerRepetitionBehavior()
{
    switch (m_typeOfRepetition) {
    case TypeOfRepetition::NEVER: {
        m_timeGone = m_duration;
        emit stopAllTimers();
        break;
    }
    case TypeOfRepetition::ONCE: {
        if (m_timeGone >= m_duration) {
            emit stopAllTimers();
        }
        break;
    }
    case TypeOfRepetition::CONTINUING: {
        if (m_timeGone >= m_duration) {
            m_timeGone = 0;
        }
        break;
    }
    }
}

bool AnimatedArrow::isAnyTimerActive()
{
    if (m_timerGrow->isActive() || m_timerFadeIn->isActive() || m_timerFadeOut->isActive()
        || m_timerFadeInAndOut->isActive() || m_timerFadeFromStartToEnd->isActive()) return true;
    return false;
}

void AnimatedArrow::timerGrowTimeOut()
{
    switchTimerRepetitionBehavior();

    if (m_timeGone == 0) {
        m_timeGone += INTERVAL;
        return;
    }

    QLineF lineSegment;
    QLineF lineFromStartToEnd(m_start, m_end);
    lineSegment.setP1(m_start);
    lineSegment.setAngle(lineFromStartToEnd.angle());
    lineSegment.setLength(1.0 * m_timeGone / m_duration * lineFromStartToEnd.length());

    QPixmap pixmap(m_size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(m_pen);
    painter.setBrush(QBrush(m_pen.color()));
    painter.drawPath(calculateArrowhead(lineSegment.p2(), lineSegment.angle(), m_pen.widthF()));
    double actualLineLenght = lineSegment.length() - QLineF(m_end, m_endWithoutArrowhead).length();
    if (actualLineLenght >= 0) {
        lineSegment.setLength(actualLineLenght);
        painter.drawPath(calculateSimplePath(m_start, lineSegment.p2()));
    }
    m_label->setPixmap(pixmap);

    m_timeGone += INTERVAL;
}

void AnimatedArrow::timerFadeInTimeOut()
{
    switchTimerRepetitionBehavior();

    QPixmap pixmap(m_size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(m_pen);
    painter.setBrush(QBrush(m_pen.color()));
    double opacity = 1.0 * m_timeGone / m_duration;
    painter.setOpacity(opacity);
    painter.drawPath(calculateSimplePath(m_start, m_endWithoutArrowhead));
    painter.drawPath(calculateArrowhead(m_end, QLineF(m_start, m_end).angle(), m_pen.widthF()));
    m_label->setPixmap(pixmap);

    m_timeGone += INTERVAL;
}

void AnimatedArrow::timerFadeOutTimeOut()
{
    switchTimerRepetitionBehavior();

    QPixmap pixmap(m_size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(m_pen);
    painter.setBrush(QBrush(m_pen.color()));
    double opacity = 1.0 - (1.0 * m_timeGone / m_duration);
    painter.setOpacity(opacity);
    painter.drawPath(calculateSimplePath(m_start, m_endWithoutArrowhead));
    painter.drawPath(calculateArrowhead(m_end, QLineF(m_start, m_end).angle(), m_pen.widthF()));
    m_label->setPixmap(pixmap);

    m_timeGone += INTERVAL;
}

void AnimatedArrow::timerFadeInAndOutTimeOut()
{
    switchTimerRepetitionBehavior();

    QPixmap pixmap(m_size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(m_pen);
    painter.setBrush(QBrush(m_pen.color()));

    double opacity = 0.0;

    if (m_timeGone >= m_duration / 2) {
        opacity = 1.0 - (1.0 * m_timeGone / m_duration);
    }
    else {
        opacity = 1.0 * m_timeGone / m_duration;
    }

    painter.setOpacity(2.0 * opacity);
    painter.drawPath(calculateSimplePath(m_start, m_endWithoutArrowhead));
    painter.drawPath(calculateArrowhead(m_end, QLineF(m_start, m_end).angle(), m_pen.widthF()));
    m_label->setPixmap(pixmap);

    m_timeGone += INTERVAL;
}

void AnimatedArrow::timerFadeFromStartToEndTimeOut()
{
    switchTimerRepetitionBehavior();

    QPixmap pixmap(m_size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(m_pen);
    painter.setBrush(QBrush(m_pen.color()));

    int pointCount = m_complexPath.elementCount() - 1;
    int currentElement = 1.0 * m_timeGone / m_duration * pointCount;

    double opacity = 0.0;

    for (int i = 0; i <= pointCount; ++i) {

        unsigned int errorCount = 0;
        QPainterPath pathSegment;

        opacity = 1.0 - (1.0 * i / pointCount);
        painter.setOpacity(opacity);

        if ((currentElement - i - 1) < 0) {
            ++errorCount;
        }
        else {
            pathSegment.moveTo(m_complexPath.elementAt(currentElement - i));
            pathSegment.lineTo(m_complexPath.elementAt(currentElement - i - 1));
        }

        if ((currentElement + i + 1) > pointCount) {
            ++errorCount;
        }
        else {
            pathSegment.moveTo(m_complexPath.elementAt(currentElement + i));
            pathSegment.lineTo(m_complexPath.elementAt(currentElement + i + 1));
        }

        if (errorCount == 2) break;

        painter.drawPath(pathSegment);
    }
    painter.drawPath(calculateArrowhead(m_end, QLineF(m_start, m_end).angle(), m_pen.widthF()));
    m_label->setPixmap(pixmap);

    m_timeGone += INTERVAL;
}

QWidget *AnimatedArrow::parent() const
{
    return m_parent;
}

void AnimatedArrow::setParent(QWidget *newParent)
{
    if (newParent == m_parent) return;
    emit stopAllTimers();
    m_parent = newParent;
    setStart(m_start);
    setEnd(m_end);
    startAnimation();
}

AnimatedArrow::AnimatedArrow(QWidget *widget, QPointF start, QPointF end, bool activeOnConstruction, QObject *parent)
    : QObject{parent}
    , m_parent(widget)
    , m_active(activeOnConstruction)
    , m_size(m_parent->size())
    , m_pen(QPen())
    , m_typeOfLine(TypeOfLine::STRAIGHT)
    , m_typeOfArrowhead(TypeOfArrowhead::ARROW)
    , m_angleOfArrowhead(150)
    , m_sizeOfArrowhead(10)
    , m_typeOfAnimation(TypeOfAnimation::NONE)
    , m_typeOfRepetition(TypeOfRepetition::NEVER)
    , m_duration(1000)
    , m_label(new QLabel(m_parent))
    , m_timeGone(0)
    , m_timerGrow(new QTimer(this))
    , m_timerFadeIn(new QTimer(this))
    , m_timerFadeOut(new QTimer(this))
    , m_timerFadeInAndOut(new QTimer(this))
    , m_timerFadeFromStartToEnd(new QTimer(this))
{
    setStart(start);
    setEnd(end);
    calculateEndWithoutArrowhead();

    if (m_size.width() == 0) m_size.setWidth(1);
    if (m_size.height() == 0) m_size.setHeight(1);

    QObject::connect(this, &AnimatedArrow::stopAllTimers, m_timerGrow, &QTimer::stop);
    QObject::connect(m_timerGrow, &QTimer::timeout, this, &AnimatedArrow::timerGrowTimeOut);
    QObject::connect(this, &AnimatedArrow::stopAllTimers, m_timerFadeIn, &QTimer::stop);
    QObject::connect(m_timerFadeIn, &QTimer::timeout, this, &AnimatedArrow::timerFadeInTimeOut);
    QObject::connect(this, &AnimatedArrow::stopAllTimers, m_timerFadeOut, &QTimer::stop);
    QObject::connect(m_timerFadeOut, &QTimer::timeout, this, &AnimatedArrow::timerFadeOutTimeOut);
    QObject::connect(this, &AnimatedArrow::stopAllTimers, m_timerFadeInAndOut, &QTimer::stop);
    QObject::connect(m_timerFadeInAndOut, &QTimer::timeout, this, &AnimatedArrow::timerFadeInAndOutTimeOut);
    QObject::connect(this, &AnimatedArrow::stopAllTimers, m_timerFadeFromStartToEnd, &QTimer::stop);
    QObject::connect(m_timerFadeFromStartToEnd, &QTimer::timeout, this, &AnimatedArrow::timerFadeFromStartToEndTimeOut);

    m_label->setGeometry(QRect(QPoint(0, 0), m_size));
    m_label->show();
    m_label->raise();

    startAnimation();
}

AnimatedArrow::~AnimatedArrow()
{
    if (m_label->parent() == nullptr) delete m_label;
}
