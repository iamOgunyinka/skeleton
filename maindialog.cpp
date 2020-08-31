#include "maindialog.hpp"
#include "ui_maindialog.h"
#include <QMimeData>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QStyle>

MainDialog::MainDialog(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::MainDialog)
{
  ui->setupUi(this);
  setWindowIcon(QApplication::style()->standardPixmap(QStyle::SP_FileDialogToParent ));
  setAcceptDrops( true );
  QObject::connect( ui->start_button, &QPushButton::clicked, this,
                    &MainDialog::OnStartButtonClicked );
}

MainDialog::~MainDialog()
{
  delete ui;
}

void MainDialog::dragEnterEvent(QDragEnterEvent *event)
{
  event->accept();
}

void MainDialog::dropEvent(QDropEvent *event)
{
  filename = event->mimeData()->urls()[0].toLocalFile();
  QFileInfo file_info{ filename };
  if( !file_info.isFile() ){
    return event->ignore();
  }

  event->acceptProposedAction();
  QFontMetrics metrics(ui->label->font());
  QString elidedText = metrics.elidedText(filename, Qt::ElideRight,
                                          ui->label->width());
  ui->label->setText( elidedText );
}

void MainDialog::OnStartButtonClicked()
{
  if( filename.isEmpty() ) return;
  ui->start_button->setEnabled( false );

  QFile input_file{ filename };
  if( !input_file.open( QIODevice::ReadOnly ) ){
    QMessageBox::warning( this, "Error", "Unable to open file" );
    return ui->start_button->setEnabled( true );
  }

  QString const output_filename = QFileDialog::getSaveFileName( this );
  if( output_filename.isEmpty() ){
    input_file.close();

    return ui->start_button->setEnabled( true );
  }

  QFile output_file{ output_filename };
  if( !output_file.open( QIODevice::WriteOnly | QIODevice::Append ) ){
    QMessageBox::warning( this, "Error", "Unable to open output file" );
    input_file.close();
    return ui->start_button->setEnabled( true );
  }

  QString const prepend_text = ui->prepend_line->text().trimmed();
  QTextStream input_text_stream{ &input_file };
  QTextStream output_text_stream{ &output_file };
  QString line{};

  while( input_text_stream.readLineInto( &line )){
    output_text_stream << prepend_text << line << "\n";
  }

  input_file.close();
  output_file.close();
  ui->start_button->setEnabled( true );
  QMessageBox::information( this, "Completed", "Completed successfully" );
}
