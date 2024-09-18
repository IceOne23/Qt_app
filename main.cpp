#include "mainwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QApplication>
#include <QTextEdit>
#include <QFile>
#include <vector>
#include <thread>
#include <algorithm>

// Функция сортировки строк
std::vector<QString> SORT(std::vector<QString>& stroki)
{
    std::sort(stroki.begin(), stroki.end(), [](const QString& a, const QString& b)
              {
                  return a.length() < b.length();
              });
    return stroki;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //имя и размеры окна
    w.setWindowTitle("Сортировка строк");
    w.setFixedHeight(400);
    w.setFixedWidth(400);

    // Создаем центральный виджет и компановку
    QWidget *centralWidget = new QWidget(&w);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Создаем кнопку для выбора файла
    QPushButton* btn1 = new QPushButton("Выбрать текстовый файл");
    layout->addWidget(btn1);

    // Создаем кнопку для сортировки строк
    QPushButton* btnSort = new QPushButton("Сортировать строки");
    layout->addWidget(btnSort);

    // Редактор для отображения строк файла
    QTextEdit *textEdit = new QTextEdit();
    textEdit->setReadOnly(true);  // Поле только для чтения
    layout->addWidget(textEdit);

    // Устанавливаем центральный виджет в главное окно
    w.setCentralWidget(centralWidget);

    // Вектор для хранения строк
    std::vector<QString> stroki;

    // Создаем новое окно для отображения отсортированных строк
    QMainWindow sortedWindow;
    sortedWindow.setWindowTitle("Отсортированные строки");
    sortedWindow.setFixedHeight(400);
    sortedWindow.setFixedWidth(400);

    QTextEdit *sortedTextEdit = new QTextEdit(&sortedWindow);
    sortedTextEdit->setReadOnly(true);
    sortedWindow.setCentralWidget(sortedTextEdit);

    // Соединяем сигнал нажатия кнопки с лямбда-функцией, которая вызывает диалог выбора файла
    QObject::connect(btn1, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(
            &w,
            "Выберите текстовый файл",
            "",                        // Начальная директория (может быть пустой)
            "Text Files (*.txt)"       // Фильтр файлов
            );

        // Проверка, выбран ли файл
        if (!fileName.isEmpty()) {
            qDebug() << "Выбран файл:" << fileName;

            QFile file(fileName);

            // Открываем файл для чтения
            if (!file.open(QIODevice::ReadOnly)) {
                qDebug() << "Не удалось открыть файл!";
                return;
            }

            // Очищаем предыдущий текст в редакторе
            textEdit->clear();
            stroki.clear();
            sortedTextEdit->clear();

            // Читаем файл построчно и выводим в текстовый редактор
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                stroki.push_back(line);  // Добавляем строки в вектор
                textEdit->append(line);  // Выводим строку в текстовый редактор
            }

            file.close();  // Закрываем файл
        }
    });

    // Соединяем сигнал кнопки "Сортировать" с лямбда-функцией для сортировки строк
    QObject::connect(btnSort, &QPushButton::clicked, [&]() {
        // Запускаем сортировку в отдельном потоке
        std::thread sortingThread([&stroki]() {
            SORT(stroki);  // Сортируем строки
        });


        sortingThread.join();

        for (const QString& str : stroki) {
            sortedTextEdit->append(str);  // Выводим отсортированные строки
        }

        sortedWindow.show();
    });

    // Показываем главное окно
    w.show();

    return a.exec();
}
