#include "hometemplateselection.h"
#include "ui_hometemplateselection.h"
#include "src/plugins/coreplugin/SetDefaultCreatorTheme.h"
HomeTemplateSelection::HomeTemplateSelection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeTemplateSelection)
{
    ui->setupUi(this);
    m_viewTemplate=new QtSupport::Internal::ExamplesPageWidget(true);
    connect(m_viewTemplate,&QtSupport::Internal::ExamplesPageWidget::ExampleClickAction,this,&HomeTemplateSelection::ExampleClickAction);
    connect(ui->pbViewTemplate,&QPushButton::clicked,[&](){
        m_viewTemplate->showMaximized();
    });
    m_viewTemplate->hide();

    framelessWindow.setContent(this);
    framelessWindow.setButtons(false,false,false,true);
}

HomeTemplateSelection::~HomeTemplateSelection()
{
    delete ui;
}
