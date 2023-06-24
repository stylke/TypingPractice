#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <fstream>
#include <string>
#include <QString>
#include <random>
#include <algorithm>
using namespace std::literals;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("TypingPractice");

    this->setFixedSize(900, 200);
    // 注意：为了让labelWordList与lineEdit中的文字始终对齐，要求字体必须是等宽字体
    ui->labelWordList->setFixedSize(880, 50);
    ui->labelWordList->setIndent(3);   // 右移label中的文字，使label与lineEdit的文字对齐
    ui->lineEdit->setFixedSize(880, 40);

    changeFile("D:/qt6_5_projects/TypingPractice/words/all.txt", true, 100);
    chooseWords();

    timer = std::make_unique<QTimer>();
    timer->setInterval(1000);
    connect(timer.get(), &QTimer::timeout, this, &MainWindow::on_timeout);

    setComboBoxChoices();
    ui->comboBox->setEnabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if(!timer->isActive()) timer->start();

    currentChars = currentCharsExceptThisLine + arg1.size();
    int correctCharsThisLine = 0;
    for(int i = 0; i < arg1.size(); i++){
        if(arg1[i] == thisLine[i]) correctCharsThisLine++;
    }
    currentCorrectChars = currentCorrectCharsExceptThisLine + correctCharsThisLine;

    ui->labelAccuricy->setText(QString::fromStdString("Accuricy: "s + std::to_string(
        currentCorrectChars * 1.0 / currentChars * 100) + "%"s));
    ui->progressBar->setValue(100 * currentChars / allCharsLength);

    if(arg1.size() >= currentLineCharsLength && arg1.size() != 0){
        currentCharsExceptThisLine = currentChars;
        currentCorrectCharsExceptThisLine = currentCorrectChars;

        if(currentWordIdx == allWords.size()){
            timer->stop();
            ui->lineEdit->setEnabled(false);
            return;
        }

        ui->lineEdit->clear();
        chooseWords();
    }    
}

void MainWindow::on_timeout()
{
    ++time;
    QString text;
    if(time < 60)
        text = QString::fromStdString("Time: "s + std::to_string(time) + "s"s);
    else
        text = QString::fromStdString("Time: "s + std::to_string(time/60) + "min"s
            + std::to_string(time%60) + "s"s);
    ui->labelTime->setText(text);
    ui->labelSpeed->setText(QString::fromStdString("Speed: "s + std::to_string(
        currentChars * 1.0 / time * 60) + "c/min"s));
}

void MainWindow::chooseWords()
{
    QString line;
    for(; currentWordIdx < allWords.size(); currentWordIdx++){
        if(line.size() + 1 + allWords[currentWordIdx].size() > LINE_CHAR_MAX) break;
        if(line != "") line += " ";
        line += allWords[currentWordIdx];
    }
    currentLineCharsLength = line.size();
    ui->labelWordList->setText(line);

    thisLine = line;
}

void MainWindow::changeFile(const std::string& fileName, bool random, size_t n)
{
    std::ifstream ifs(fileName);
    if(!ifs.is_open()){
        qDebug() << "Unable to open the file.";
        return;
    }
    std::string word;
    allWords.clear();
    while(ifs >> word){
        allWords.push_back(word);
    }

    if(random){
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(allWords.begin(), allWords.end(), g);
    }

    if(n != 0 && n < allWords.size()){
        allWords.erase(allWords.begin() + n, allWords.end());
    }
    // 计算allCharsLength
    allCharsLength = 0;
    QString line;
    for(size_t i = 0; i < allWords.size(); i++){
        if(line.size() + 1 + allWords[i].size() > LINE_CHAR_MAX){
            allCharsLength += line.size();
            line = "";
        }
        if(line != "") line += " ";
        line += allWords[i];
    }
    allCharsLength += line.size();

    currentWordIdx = 0;
    currentLineCharsLength = 0;
}


void MainWindow::on_pushButtonStop_clicked()
{
    if(!ui->lineEdit->isEnabled() && !stop) return; // 如果此时进度已经达到100%，则按暂停应该不起作用。注意：进度达到100%时的暂停输入没有设置stop为true
    if(!stop){
        stop = true;
        timer->stop();
        ui->lineEdit->setEnabled(false);
        ui->pushButtonStop->setText("继续");
    } else{
        stop = false;
        timer->start();
        ui->lineEdit->setEnabled(true);
        ui->pushButtonStop->setText("暂停");
    }
}


void MainWindow::on_pushButtonReset_clicked()
{
    reset();
}


void MainWindow::setComboBoxChoices()
{
    ui->comboBox->addItem("random");
    for(int i = 0; i < 26; i++){
        std::string c = " ";
        c[0] = 'a' + i;
        ui->comboBox->addItem(QString::fromStdString(c));
    }
}

void MainWindow::reset()
{
    currentWordIdx = 0;
    currentLineCharsLength = 0;

    time = -1;
    on_timeout();

    currentCharsExceptThisLine = 0;
    currentCorrectCharsExceptThisLine = 0;
    currentChars = 0;
    currentCorrectChars = 0;

    if(!ui->lineEdit->isEnabled()){
        stop = false;
        ui->lineEdit->setEnabled(true);
        ui->pushButtonStop->setText("暂停");
    }

    chooseWords();
    ui->lineEdit->clear();

    timer->stop();  // 等到用户输入第一个字符时才开始计时
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    if(arg1 == "random"){
        fileName = "all.txt";
        changeFile(dir + fileName, true, 100);
    } else{
        std::string str = arg1.toStdString();
        char c = str[0];
        fileName = c + ".txt"s;
        changeFile(dir + fileName);
    }
    reset();
}

