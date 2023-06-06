#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include "do_aes.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    uint8_t in[16] = {0};
    uint8_t out1[16] = {0};
    uint8_t out2[16] = {0};
    QString input = ui->textEdit->toPlainText();

    QByteArray array = QByteArray::fromHex(input.toLatin1());
	uint8_t* data = reinterpret_cast<uint8_t*>(array.data());
    memcpy(in, data, std::min((int)array.size(), 16));

    if(ui->radioButton_2->isChecked()) {
        int ret = Aes::tee_domain_aes(in, out1, out2);
        if(ret != 0) {
            QMessageBox::warning(this, "Error", "Failed to Aes::tee_domain_aes.");
        }
    } else if(ui->radioButton->isChecked()){
        int ret = Aes::dircall_aes(in, out1, out2);
        if(ret != 0) {
            QMessageBox::warning(this, "Error", "Failed to Aes::dircall_aes.");
        }
    }

	input = QByteArray::fromRawData(reinterpret_cast<char*>(in), 16).toHex(' ');
    QString output1 = QByteArray::fromRawData(reinterpret_cast<char*>(out1), 16).toHex(' ');
    QString output2 = QByteArray::fromRawData(reinterpret_cast<char*>(out2), 16).toHex(' ');
    ui->textEdit->setPlainText(input);
    ui->textEdit_2->setPlainText(output1);
    ui->textEdit_3->setPlainText(output2);
}

