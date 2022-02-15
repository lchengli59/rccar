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
#include <QtWidgets>

#include <QFileDialog>
#include <QImageReader>
#include <QLabel>

/*-------------------------------------------------------------------------*/
#include "mallmwdw.h"

/*-------------------------------------------------------------------------*/
mallMwdw::mallMwdw(QWidget *parent)
    : QMainWindow(parent), imageLabel(new QLabel)
/*-------------------------------------------------------------------------*/
{
    selMdl = "NH";
    selIdx = 1;


    // a) create actions
    createActions();

    // b) create menus
    createMenus();

    // c) create status bar
    createStatusBar();

    // d) create dock windows
    createDockWindows();

    // e) create window title
    sprintf(title,"Car Game");
    setWindowTitle(tr(title));

    /*-
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(
        QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    QString fileName = "background.jpg";
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    image = reader.read();
    if (image.isNull()) {
        QMessageBox::information(this, 
        QGuiApplication::applicationDisplayName(),
        tr("Cannot load %1: %2").arg(
        QDir::toNativeSeparators(fileName), reader.errorString()));
        //return false;
    }
    */

    //scene = new QGraphicsScene;
    scene = new DiagramScene;
    scene->setSceneRect(0, 0, 1280, 720);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    //view->setBackgroundBrush(Qt::white);
    view->setBackgroundBrush(Qt::black);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    /*
    view->setBackgroundBrush(QPixmap::fromImage(image));
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Colliding Mice"));
    view->resize(1300, 750);
    */
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);

}

/*-------------------------------------------------------------------------*/
mallMwdw::~mallMwdw()
/*-------------------------------------------------------------------------*/
{
}

/*-------------------------------------------------------------------------*/
void mallMwdw::about()
/*-------------------------------------------------------------------------*/
{
    char msgTitle[50];
    char msgPurpose[200];
    sprintf(msgTitle,"About %s", title);
    sprintf(msgPurpose,
        "The <b>Graphical User interface (GUI)</b> \
        contain various docking windows \
        for showing status, control and locator for <b>%s</b>", title);
    QMessageBox::about(this, tr(msgTitle), tr(msgPurpose));
}

/*-------------------------------------------------------------------------*/
void mallMwdw::createActions()
/*-------------------------------------------------------------------------*/
{
    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcut(tr("Ctrl+Q"));
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

/*-------------------------------------------------------------------------*/
void mallMwdw::createMenus()
/*-------------------------------------------------------------------------*/
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

}

/*-------------------------------------------------------------------------*/
void mallMwdw::createStatusBar()
/*-------------------------------------------------------------------------*/
{
    statusBar()->showMessage(tr("Ready"));
}

/*-------------------------------------------------------------------------*/
void mallMwdw::createDockWindows()
/*-------------------------------------------------------------------------*/
{

}
