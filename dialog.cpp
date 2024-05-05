#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QColorDialog>
#include <QFileDialog>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),

    ui(new Ui::Dialog) {
    ui->setupUi(this);

    model = new QDirModel (this);
    model->setReadOnly (false);
    model->setSorting((QDir::DirsFirst | QDir::IgnoreCase | QDir::Name));


    ui->treeView->setModel(model);
    ui->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    QModelIndex index = model->index("C://");

    ui->treeView->expand(index);
    ui->treeView->scrollTo(index);
    ui->treeView->setCurrentIndex((index));
    ui->treeView->resizeColumnToContents(0);

}

Dialog::~Dialog() {
    delete ui;
}

//создать папку
void Dialog::on_pushButton_clicked() {
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) return;

    QString name =QInputDialog::getText(this,"Новая папка","Введите название папки!");

    if (name.isEmpty()) return;

    model->mkdir(index, name);
}

QStringList copiedFiles;

//копировать файл
void Dialog::on_pushButton_2_clicked() {
    QModelIndexList indexes = ui->treeView->selectionModel()->selectedIndexes();

    // Проверяем, что список не пустой
    if (indexes.isEmpty()) {
        QMessageBox::warning(this, tr("Внимание!!"), tr("Ничего не было выбрано."));
        return;
    }

    // Очищаем список скопированных файлов перед новым копированием
    copiedFiles.clear();

    // Проходим по списку индексов и копируем каждый элемент
    foreach (QModelIndex index, indexes) {
        // Получаем путь к файлу или директории
        QString filePath = model->filePath(index);

        // Копируем файл или директорию
        // Путь к новой копии будет формироваться автоматически с добавлением "_copy"
        QFileInfo fileInfo(filePath);
        QString destPath = fileInfo.absolutePath() + "/" + fileInfo.fileName() + "_copy";
        if (QFile::copy(filePath, destPath)) {
            copiedFiles.append(destPath); // Добавляем скопированный файл в список
        } else {
            qDebug() << "Failed to copy file: " << filePath;
        }
    }

    // Сообщаем пользователю об успешном копировании
    QMessageBox::information(this, tr("Копировать"), tr("Файлы успешно скопированы!!"));
}

//вставить
void Dialog::on_pushButton_3_clicked() {
    if (copiedFiles.isEmpty()) {
        QMessageBox::warning(this, tr("Внимание!!"), tr("Ничего не было скопировано."));
        return;
    }

    // Показываем диалоговое окно для ввода пути
    QString destPath = QInputDialog::getText(this, tr("Выберите каталог назначения?"),
                                             tr("Выберите путь:"), QLineEdit::Normal, "", nullptr, Qt::WindowFlags());

    // Проверяем, что пользователь ввел путь
    if (destPath.isEmpty()) {
        return;
    }

    // Перемещаем каждый скопированный файл в выбранную папку
    foreach (QString copiedFile, copiedFiles) {
        QFileInfo fileInfo(copiedFile);
        QString destFilePath = destPath + "/" + fileInfo.fileName();
        if (QFile::copy(copiedFile, destFilePath)) {
            // Успешно скопировано
        } else {
            qDebug() << "Failed to copy file: " << copiedFile;
        }
    }

    // Очищаем список скопированных файлов
    copiedFiles.clear();

    // Сообщаем пользователю об успешной вставке
    QMessageBox::information(this, tr("Вставить"), tr("Файлы успешно вставлены!!"));

    // Обновляем отображение treeView
    model->refresh();
}

//удалить
void Dialog::on_pushButton_4_clicked() {
    // Получаем список выбранных индексов
    QModelIndexList indexes = ui->treeView->selectionModel()->selectedIndexes();

    // Проверяем, что список не пустой
    if (indexes.isEmpty()) {
        QMessageBox::warning(this, tr("Внимание!!"), tr("Ничего не было выбрано."));
        return;
    }

    // Проходим по списку индексов в обратном порядке и удаляем каждый элемент
    for (int i = indexes.size() - 1; i >= 0; --i) {
        const QModelIndex &index = indexes.at(i);
        if (model->fileInfo(index).isDir()) {
            // Удаляем директорию
            model->rmdir(index);
        } else {
            // Удаляем файл
            model->remove(index);
        }
    }

}

//окно справки
void Dialog::on_pushButton_5_clicked()
{
    //info
    QMessageBox* pmbx =new QMessageBox
            ("Справочка)","Это мог быть нормальный файловый менеджер",QMessageBox::Information,
             QMessageBox::NoButton,
             QMessageBox::Ok,
             QMessageBox::NoButton
              | QMessageBox::Escape);
    int n = pmbx->exec();
    delete pmbx;
}

//выход
void Dialog::on_pushButton_6_clicked() {
    QApplication::quit();
}
