#ifndef MAINDIALOG_HPP
#define MAINDIALOG_HPP

#include <QDialog>
#include <QDragEnterEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainDialog; }
QT_END_NAMESPACE

class MainDialog : public QDialog
{
  Q_OBJECT

  void OnStartButtonClicked();
public:
  MainDialog(QWidget *parent = nullptr);
  ~MainDialog();
  void dragEnterEvent( QDragEnterEvent* ) override;
  void dropEvent( QDropEvent * ) override;
private:
  Ui::MainDialog *ui;
  QString filename {};
};
#endif // MAINDIALOG_HPP
