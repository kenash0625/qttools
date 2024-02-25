#include "qtsupport_global.h"
#include <QLineEdit>
#include <QPointer>
namespace Core {
class GridProxyModel;
}
namespace QtSupport {
namespace Internal {
class ExamplesListModel;
class ExampleDelegate;
class QTSUPPORT_EXPORT ExamplesPageWidget : public QWidget
{
    Q_OBJECT
public:
    ExamplesPageWidget(bool isExamples);

    int bestColumnCount() const;

    void resizeEvent(QResizeEvent *ev) final;

    void onTagClicked(const QString &tag);

    const bool m_isExamples;
    QPointer<ExampleDelegate> m_exampleDelegate;
    QPointer<ExamplesListModel> m_examplesModel;
    QLineEdit *m_searcher;
    QPointer<Core::GridProxyModel> m_gridModel;

signals:
    void ExampleClickAction(const QString&exampleFile);
};
} // namespace Internal
} // namespace QtSupport
