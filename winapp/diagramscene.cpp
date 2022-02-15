/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*-------------------------------------------------------------------------*/
#include <QTextCursor>
#include <windows.h>

/*-------------------------------------------------------------------------*/
#include "diagramscene.h"


/*-------------------------------------------------------------------------*/
DiagramScene::DiagramScene(QObject *parent)
    : QGraphicsScene(parent)
/*-------------------------------------------------------------------------*/
{
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;

    mouse = new Mouse;
    mouse->setPos(10*32,10*32);
    this->addItem(mouse);

    car = new myCar(10, 10);
    startTimer(100);
    dt = 0.1;

}

/*-------------------------------------------------------------------------*/
void DiagramScene::setLineColor(const QColor &color)
/*-------------------------------------------------------------------------*/
{
    myLineColor = color;
}

/*-------------------------------------------------------------------------*/
void DiagramScene::setTextColor(const QColor &color)
/*-------------------------------------------------------------------------*/
{
    myTextColor = color;
}

/*-------------------------------------------------------------------------*/
void DiagramScene::setItemColor(const QColor &color)
/*-------------------------------------------------------------------------*/
{
    myItemColor = color;
}

/*-------------------------------------------------------------------------*/
void DiagramScene::setFont(const QFont &font)
/*-------------------------------------------------------------------------*/
{
    myFont = font;

}

/*-------------------------------------------------------------------------*/
void DiagramScene::setMode(Mode mode)
/*-------------------------------------------------------------------------*/
{
    myMode = mode;
}

/*-------------------------------------------------------------------------*/
bool DiagramScene::isItemChange(int type) const
/*-------------------------------------------------------------------------*/
{
    const QList<QGraphicsItem *> items = selectedItems();
    const auto cb = [type](const QGraphicsItem *item) {
     return item->type() == type; 
    };
    return std::find_if(items.begin(), items.end(), cb) != items.end();
}

/*-------------------------------------------------------------------------*/
void DiagramScene::timerEvent(QTimerEvent *evt)
/*-------------------------------------------------------------------------*/
{
    Q_UNUSED(evt);

    if (GetKeyState(VK_UP)<0)
        car->forward(dt);
    else if (GetKeyState(VK_DOWN)<0)
        car->backward(dt);
    else
        car->handsoff(dt);

    if (GetKeyState(VK_RIGHT)<0)
        car->steerwheel("RIGHT", dt);
    else if (GetKeyState(VK_LEFT)<0)
        car->steerwheel("LEFT", dt);
    else
        car->steerwheel("NONE", dt);

    car->update(dt);
    mouse->updateState(car->angle, car->position);


}
