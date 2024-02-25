#ifndef HOMETEMPLATESELECTION_H
#define HOMETEMPLATESELECTION_H

#include <QWidget>
#include "src/plugins/qtsupport/ExamplesPageWidget.h"
#include "../Qt-Frameless-Window-DarkStyle-master/framelesswindow/framelesswindow.h"

#include"src/libs/extensionsystem/pluginmanager.h"
namespace Ui {
class HomeTemplateSelection;
}

class HomeTemplateSelection : public QWidget
{
    Q_OBJECT

public:
    explicit HomeTemplateSelection(QWidget *parent = nullptr);
    ~HomeTemplateSelection();
    Ui::HomeTemplateSelection *ui;
    QtSupport::Internal::ExamplesPageWidget* m_viewTemplate;
    FramelessWindow framelessWindow;
signals:
    void ExampleClickAction(const QString&exampleFile);
private:
};

#endif // HOMETEMPLATESELECTION_H
