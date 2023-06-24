#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <vector>
#include <QTimer>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

constexpr int LINE_CHAR_MAX = 99;  // 每一行最多容纳的字符数

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);
    void on_timeout();

    void on_pushButtonStop_clicked();

    void on_pushButtonReset_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    std::vector<std::string> allWords;
    int allCharsLength;
    QString thisLine;
    size_t currentWordIdx;
    int currentLineCharsLength;

    void chooseWords();
    void changeFile(const std::string& fileName, bool random=false, size_t n=0);

    std::unique_ptr<QTimer> timer;
    int time {0};

    int currentCharsExceptThisLine {0};
    int currentCorrectCharsExceptThisLine {0};
    int currentChars {0};
    int currentCorrectChars {0};

    bool stop {false};

    std::string dir = "D:/qt6_5_projects/TypingPractice/words/";
    std::string fileName = "w.txt";
    void setComboBoxChoices();

    void reset();
};
#endif // MAINWINDOW_H
